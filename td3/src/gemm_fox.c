#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "util.h"
#include "gemm.h"

#define STDOUT 1
#define TEST {printf("TEST %d ME : %d \n",__LINE__,rank);fflush(stdout);}


/* C = A*B algorithme de fox 
   dim(A)=dim(B)=N=n*k
   colonne major
*/
int main(int argc, char** argv){
  MPI_Init(NULL,NULL);
  int rank, size, tag;
  int root = 0;
  tag = 99;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* variables used in every process */
  int dim, dim_A,dim_B, size_blocs, nb_div;
  double *A;
  double * B;

  if(rank == root){
    if (argc != 4) {
      printf("Erreur : Argument manquant\n");
      printf("Usage : ./exec file_A file_B file_C\n");
      return EXIT_FAILURE;
    } 
    
    /* Matrix A file */
    FILE* fd_A =fopen(argv[1], "r");
    if (!fd_A){
      perror("Erreur ouverture fichier\n");
      return EXIT_FAILURE;
    } 
    
    /* Matrix B file */
    FILE* fd_B =fopen(argv[2], "r");
    if (!fd_B){
      perror("Erreur ouverture fichier\n");
      return EXIT_FAILURE;
    } 
    char ligne[MAX_LINE];
    double *A,B;
    fgets(ligne, MAX_LINE, fd_A);
    dim_A = atoi(ligne);
    fgets(ligne, MAX_LINE, fd_B);
    dim_B = atoi(ligne);
    if(dim_A != dim_B){
      fprintf(stderr, "A and B must have same dimensions");
      return EXIT_FAILURE;
    }    
    else if(dim_A% (int) sqrt(size) != 0){
      fprintf(stderr, "Wrong dimensions");
      return EXIT_FAILURE;
    }    
    fclose(fd_A);fclose(fd_B);    
    dim = dim_A;
  }
  /* broadcasting size, probably better than every nodes accessing the file */
  
  MPI_Bcast(&dim, 1, MPI_INT, root, MPI_COMM_WORLD);
  nb_div = (int) sqrt(size);
  size_blocs = dim/nb_div;
  
  /* creating cartesien communicators */
  MPI_Status status;
  int dims[2];dims[0]=nb_div;dims[1]=nb_div;
  int periods[2];periods[0]=1;periods[1]=0;
  int reorder=1;
  MPI_Comm grid_comm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &grid_comm);
    
  int coord[2];
  MPI_Cart_coords(grid_comm, rank, 2, coord);
  int myrow=coord[0], mycol=coord[1];
  /* row communicator */
  MPI_Comm row_comm;
  int remain[2];
  remain[0] = 0;
  remain[1] = 1;
  MPI_Cart_sub(grid_comm, remain, &row_comm);

  /* col communicator */
  MPI_Comm col_comm;
  remain[0] = 1;
  remain[1] = 0;
  MPI_Cart_sub(grid_comm, remain, &col_comm);

  /* block datatype */
  MPI_Datatype bloc;
  MPI_Type_vector(size_blocs,size_blocs,dim,MPI_DOUBLE, &bloc);
  MPI_Type_commit(&bloc);
  
  
  if( rank == root){
    A = (double *) malloc(sizeof(double)*dim*dim);
    B = (double *) malloc(sizeof(double)*dim*dim);
    parse_matrix(argv[1],A);
    parse_matrix(argv[2],B);
   
    /* MATRIX ARE LOADED */
    /* cut a matrix into blocks and send it to cartesien grid process */   
    int world_rank;
    int coord[2];
    for(int i=0; i<nb_div; ++i){
      for(int j=0; j<nb_div; ++j){
	coord[0]=i;coord[1]=j;
	MPI_Cart_rank(grid_comm, coord, &world_rank);
	MPI_Send(A+i*size_blocs+j*dim*size_blocs,1,bloc,world_rank,tag, MPI_COMM_WORLD);
	MPI_Send(B+i*size_blocs+j*dim*size_blocs,1,bloc,world_rank,tag, MPI_COMM_WORLD);
      }
    }
  }
  //MPI_Barrier(MPI_COMM_WORLD);


  /* starting fox product, consider this section as local */
  /* receiving the local blocks A B */
  double * lblocA = (double *) malloc(sizeof(double)*size_blocs*size_blocs);
  double * lblocB = (double *) malloc(sizeof(double)*size_blocs*size_blocs);
  MPI_Recv(lblocA,size_blocs*size_blocs,MPI_DOUBLE,0,tag,MPI_COMM_WORLD, &status);
  MPI_Recv(lblocB,size_blocs*size_blocs,MPI_DOUBLE,0,tag,MPI_COMM_WORLD, &status);
 
  /* result block Cij */
  double * lblocC = (double *) malloc(sizeof(double)*size_blocs*size_blocs);
  for(int i =0; i<size_blocs*size_blocs; ++i)
    lblocC[i] = 0.0;
  /* blocks for broacast */
  double * curA = (double *) malloc(sizeof(double)*size_blocs*size_blocs);
  
  /* local product */
  int to_broadcast;
  /* circular rotation for b blocks */
  int src,dest;
  MPI_Cart_shift(col_comm,0,1,&dest,&src);
  for(int k=0; k<nb_div; ++k){
    /* process to broadcast A block */
    to_broadcast = (myrow + k)%nb_div;
   
    if(mycol == to_broadcast){
      MPI_Bcast(lblocA, size_blocs*size_blocs, MPI_DOUBLE, to_broadcast, row_comm);
      gemm(size_blocs,size_blocs,size_blocs,lblocA,size_blocs,lblocB,size_blocs,
	   lblocC, size_blocs);
    }
    else{

      MPI_Bcast(curA, size_blocs*size_blocs, MPI_DOUBLE, to_broadcast, row_comm);
      gemm(size_blocs,size_blocs,size_blocs,curA,size_blocs,lblocB,size_blocs,
	   lblocC, size_blocs);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    /* sending b blocks with rotation */
    MPI_Sendrecv_replace(lblocB,size_blocs*size_blocs, MPI_DOUBLE,dest,tag,src,tag,col_comm,&status);
        
 }
  int displs[size];
  
  for(int i=0; i<size; ++i){	 
    MPI_Cart_coords(grid_comm,i, 2, coord);
    displs[i] = coord[0]+coord[1]*size_blocs*size_blocs;
  }
  double * C;
  if( rank == root){
    C = (double *) malloc(sizeof(double)*dim*dim);
  }
  /* gather results */
  int recvcounts[size];
  for(int i=0; i<size; ++i)
    recvcounts[i] = 1;
  MPI_Datatype bloc_resized;
  MPI_Type_create_resized(bloc,(MPI_Aint) 0, (MPI_Aint) size_blocs*sizeof(double),&bloc_resized);
  MPI_Type_commit(&bloc_resized);
  MPI_Gatherv(lblocC,size_blocs*size_blocs,MPI_DOUBLE,C,recvcounts,displs,bloc_resized,root,MPI_COMM_WORLD);
  if(rank == root){
    /* copy on a file */
        /* Matrix B file */
    int fdc = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC,0744);
    if (fdc < 0){
      perror("Erreur ouverture fichier\n");
      return EXIT_FAILURE;
    } 
    print_matrix(C,dim,fdc);
    close(fdc);
    for(int i =0; i<dim*dim; ++i)
      C[i] = 0.0;    
    gemm(dim,dim,dim,A,dim,B,dim,C,dim);
    print_matrix(C,dim,1);
    free(C);
    free(A);
    free(B);    
  }
  free(lblocA);
  free(lblocB);
  free(lblocC);
  free(curA);

  MPI_Finalize();
  return 0;
}
