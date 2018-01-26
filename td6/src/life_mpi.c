#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
int BS;

#define cell( _i_, _j_ ) board[ ldboard * (_j_) + (_i_) ]
#define ngb( _i_, _j_ )  nbngb[ ldnbngb * ((_j_) - 1) + ((_i_) - 1 ) ]
#define cell_loc( _i_, _j_ ) board_loc[ ldboard_loc * (_j_) + (_i_) ]
#define ngb_loc( _i_, _j_ )  nbngb_loc[ ldnbngb_loc * ((_j_) - 1) + ((_i_) - 1 ) ]


double mytimer(void)
{
  struct timeval tp;
  gettimeofday( &tp, NULL );
  return tp.tv_sec + 1e-6 * tp.tv_usec;
}

void output_board_loc(int N, int *board_loc, int ldboard_loc, int loop)
{
  int i,j;
  printf("loop %d\n", loop);
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      //     printf(" %d", cell_loc( i, j ));
      if ( cell_loc( i, j ) == 1)
      	printf("X");
      else
      	printf(".");
    }
    printf("\n");
  }
}

void output_board(int N, int *board, int ldboard, int loop)
{
  int i,j;
  printf("loop %d\n", loop);
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      if ( cell( i, j ) == 1)
      	printf("X");
      else
      	printf(".");
   }
    printf("\n");
  }
}

/**
 * This function generates the iniatl board with one row and one
 * column of living cells in the middle of the board
 */
int generate_initial_board(int N, int *board, int ldboard)
{
  int i, j, num_alive = 0;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (i == N/2 || j == N/2) {
	cell(i, j) = 1;
	num_alive ++;
      }
      else {
	cell(i, j) = 0;
      }
    }
  }

  return num_alive;
}

int main(int argc, char* argv[])
{
  int i, j, loop, num_alive, maxloop;
  int ldboard, ldnbngb,ldboard_loc, ldnbngb_loc;
  double t1, t2;
  double temps;

  int *board, *board_loc;
  int *nbngb, *nbngb_loc;

  if (argc < 3) {
    printf("Usage: %s nb_iterations size\n", argv[0]);
    return EXIT_SUCCESS;
  } else {
    maxloop = atoi(argv[1]);
    BS = atoi(argv[2]);
    //printf("Running sequential version, grid of size %d, %d iterations\n", BS, maxloop);
  }
  MPI_Init(NULL,NULL);
  int rank,size,tag,root;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  root = 0;
  //
  int nb_bloc = (int) sqrt(size);
  fprintf(stderr, "size %d, nb_bloc %d\n",size, nb_bloc);
  if (BS % nb_bloc != 0){
    printf("Usage: Wrong size and number of processes; sqrt(np) must divide size");
    MPI_Finalize();
    return EXIT_SUCCESS;
  }
      
  /* Creating grid */
  MPI_Comm grid;
  int ndims = 2; int reorder=1;
  int dims[] = {nb_bloc,nb_bloc};
  int periods[] = {1,1};
  MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,reorder,&grid);
  int bbs = BS/nb_bloc;
  
  num_alive = 0;
  /* Leading dimension of the board array */
  ldboard = BS + 2;
  /* Leading dimension of the neigbour counters array */
  ldnbngb = BS;
  ldboard_loc = bbs + 2;
  ldnbngb_loc = bbs;
 
  /* block datatype */
  MPI_Datatype bloc,bloc_loc,bloc_resized;
  MPI_Type_vector(bbs,bbs,ldboard,MPI_INT,&bloc);
  MPI_Type_vector(bbs,bbs,ldboard_loc,MPI_INT,&bloc_loc);
  MPI_Type_commit(&bloc);
  MPI_Type_commit(&bloc_loc);
// bbs * sizeof(int) à verifier
  MPI_Type_create_resized(bloc,(MPI_Aint) 0, (MPI_Aint) bbs*sizeof(int),&bloc_resized);
  MPI_Type_commit(&bloc_resized);

  if (rank == root){
    board = calloc(sizeof(int), ldboard * ldboard );
    nbngb = calloc(sizeof(int), ldnbngb * ldnbngb);
    num_alive = generate_initial_board( BS, &(cell(1, 1)), ldboard );
  }
  board_loc = calloc(sizeof(int),ldboard_loc * ldboard_loc);
  nbngb_loc = calloc(sizeof(int),ldnbngb_loc * ldnbngb_loc);
 
  /* Transmitting blocs */
  MPI_Bcast(&num_alive,1,MPI_INT,root,MPI_COMM_WORLD);
  /* MPI_Status status; */
  /* if (rank == 0){ */
  /*   MPI_Send(&(cell(1,3)),1,bloc,3,99,MPI_COMM_WORLD); */
  /*   output_board(ldboard,&(cell(0, 0)),ldboard,rank); */
  /* } */
  /* int tmp[4]; */
  /* MPI_Barrier(MPI_COMM_WORLD); */
  /* if (rank == 3){ */
  /*   MPI_Recv(&(cell_loc(1,1)),1,bloc_loc,0,99,MPI_COMM_WORLD,&status); */
  /*   /\* for (int j=0; j < 4; j++){ *\/ */
  /*   /\*   printf("%d ", tmp[j]); *\/ */
  /*   /\* } *\/ */
  /*  output_board_loc(ldboard_loc,&(cell_loc(0, 0)),ldboard_loc,rank); */
  /* } */
  
  int sendcounts[size],displs[size];
  int dis = 0;
  int coord[2];
  for (int i=0; i < size; i++){
    sendcounts[i] = 1;
    MPI_Cart_coords(grid,i,2,coord);
    displs[i] = coord[0]*bbs + coord[1]*bbs*ldboard;
   /* if ((i%nb_bloc == 0) && (i!=0)) dis += nb_bloc*bbs*bbs; */
    /* displs[i] = dis; */
    /* dis += bbs; */
  }
  
  printf("ldboard_loc %d\n", ldboard_loc);
  MPI_Scatterv(&(cell(1, 1)),sendcounts,displs,bloc_resized,&(cell_loc(1, 1)),1,bloc_loc,root,MPI_COMM_WORLD);
  
  if (rank == root){
    printf("Starting number of living cells = %d\n", num_alive);
    output_board( BS, &(cell(1, 1)), ldboard, loop);
 }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 1)
    output_board_loc(ldboard_loc,&(cell_loc(0, 0)),ldboard_loc,rank);
 
  
  
/*  // AllReduce */
 /*  t1 = mytimer(); */
 /*  for (loop = 1; loop <= maxloop; loop++) { */
    

 /*    cell_loc(   0, 0   ) = cell_loc((BS/size), (BS/size)); */
 /*    cell_loc(   0, (BS/size)+1) = cell_loc((BS/size),  1); */
 /*    cell_loc((BS/size)+1, 0   ) = cell_loc( 1, (BS/size)); */
 /*    cell_loc((BS/size)+1, (BS/size)+1) = cell_loc( 1,  1); */

    
 /*    for (i = 1; i <= (BS/size); i++) { */
 /*      cell_loc(   i,    0) = cell_loc( i, (BS/size)); */
 /*      cell_loc(   i, (BS/size)+1) = cell_loc( i,  1); */
 /*      cell_loc(   0,    i) = cell_loc((BS/size),  i); */
 /*      cell_loc((BS/size)+1,    i) = cell_loc( 1,  i); */
 /*    } */


 /*    for (j = 1; j <= BS/size; j++) { */
 /*      for (i = 1; i <= BS/size; i++) { */
 /* 	ngb_loc( i, j ) = */
 /* 	  cell_loc( i-1, j-1 ) + cell_loc( i, j-1 ) + cell_loc( i+1, j-1 ) + */
 /* 	  cell_loc( i-1, j   ) +                  cell_loc( i+1, j   ) + */
 /* 	  cell_loc( i-1, j+1 ) + cell_loc( i, j+1 ) + cell_loc( i+1, j+1 ); */
 /*      } */
 /*    } */

 /*    num_alive = 0; */
 /*    for (j = 1; j <= BS/size; j++) { */
 /*      for (i = 1; i <= BS/size; i++) { */
 /* 	if ( (ngb_loc( i, j ) < 2) || */
 /* 	     (ngb_loc( i, j ) > 3) ) { */
 /* 	  cell_loc(i, j) = 0; */
 /* 	} */
 /* 	else { */
 /* 	  if ((ngb_loc( i, j )) == 3) */
 /* 	    cell_loc(i, j) = 1; */
 /* 	} */
 /* 	if (cell_loc(i, j) == 1) { */
 /* 	  num_alive ++; */
 /* 	} */
 /*      } */
 /*    } */

 /*    /\* Avec les celluls sur les bords (utile pour vérifier les comm MPI) *\/ */
 /*    /\* output_board( BS+2, &(cell(0, 0)), ldboard, loop ); *\/ */

 /*    /\* Avec juste les "vraies" cellules: on commence à l'élément (1,1) *\/ */
 /*    /\*output_board( BS, &(cell(1, 1)), ldboard, loop);*\/ */

 /*    printf("%d cells are alive\n", num_alive); */
 /*  } */

 /*  t2 = mytimer(); */
 /*  temps_loc = t2 - t1; */
 /*  if (rank == root){ */
 /*    printf("Final number of living cells = %d\n", num_alive); */
 /*    printf("%.2lf\n",(double)temps * 1.e3); */
 /*  } */
  if (rank == 0){
    free(board);
    free(nbngb);
  }
  free(board_loc);
  free(nbngb_loc);
  MPI_Finalize();
  return EXIT_SUCCESS;
}

