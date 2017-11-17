#include <stdio.h>
#include <util.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

/* C = A*B algorithme de fox 
   dim(A)=dim(B)=N=n*k
   colonne major
*/
int main(int argc, char** argv){
  if (argc != 3) {
    printf("Erreur : Argument manquant\n");
    printf("Usage : ./exec file_A file_B\n");
    return EXIT_FAILURE;
  } 
  
  MPI_Init(NULL,NULL);
  int rank, size, tag;
  int root = 0;
  tag = 99;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  if(rank == 0){
    /* Matrix A file */
    FILE* fd_A =fopen(argv[1], "r");
    if (!fd){
      perror("Erreur ouverture fichier\n");
      return EXIT_FAILURE;
    } 
    
    /* Matrix B file */
    FILE* fd_B =fopen(argv[2], "r");
    if (!fd){
      perror("Erreur ouverture fichier\n");
      return EXIT_FAILURE;
    } 
    int dim_A,dim_B, n;
    char ligne[MAX];
    double *A,B;
    fgets(ligne, MAX, fd_A);
    dim_A = atoi(ligne);
    fgets(ligne, MAX, fd_B);
    dim_B = atoi(ligne);
    if(dim_A != dim_B){
      fprintf(stderr, "A and B must have same dimensions");
      return EXIT_FAILURE;
    }
    else if(dim_A%sqrt(p) == 0){
      fprintf(stderr, "Wrong dimensions");
      return EXIT_FAILURE;
    }
    fclose(fd_A);fclose(fd_B);
    n = dim_A;
    A = malloc(sizeof(double)*n*n);
    B = malloc(sizeof(double)*n*n);
    parse_matrix(argv[1],A);
    parse_matrix(argv[1],B);
    MPI_Datatype block;
    
    int blocklengths_p[2] = {1,1};
    MPI_Datatype types_p[2] =  {MPI_INT, Vecteur_d};
    MPI_Aint offsets_p[2];
    offsets_p[0] = offsetof(particule,m);
    offsets_p[1] = offsetof(particule,p);
    MPI_Type_create_struct(2,blocklengths_p,offsets_p,types_p,&Particule_d);
    MPI_Type_commit(&Particule_d);
  }
 
 
  
}
