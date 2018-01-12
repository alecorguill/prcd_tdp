#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

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
  int ldboard, ldnbngb;
  double t1, t2;
  double temps;

  int *board;
  int *nbngb;

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
  MPI_Comm_rank(MPI_COMM_WORLD,&size);
  root = 0;
  //
  if (BS % size != 0){
    printf("Usage: Need a multiple of size for the number of processes");
    MPI_Finalize();
    return EXIT_SUCCESS;
  }
    
  
  
  // Creating grid
  MPI_Comm grid;
  int ndims = 2; int reorder=1;
  int nb_bloc = (int) sqrt(size);
  int dims[] = {nb_bloc,nb_bloc};
  int periods = {1,1};
  MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,reorder,&grid);
  
  num_alive = 0;
  /* Leading dimension of the board array */
  ldboard = BS + 2;
  /* Leading dimension of the neigbour counters array */
  ldnbngb = BS;
  if (rank == root){
    board = malloc( ldboard * ldboard * sizeof(int) );
    nbngb = malloc( ldnbngb * ldnbngb * sizeof(int) );
    num_alive = generate_initial_board( BS, &(cell(1, 1)), ldboard );
 }
  MPI_Broadcast(&num_alive,1,MPI_INT,root,MPI_COMM_WORLD);
  ldboard_loc = BS/size + 2;
  ldnbngb_loc = BS/size;
  board_loc = malloc( ldboard_loc * ldboard_loc * sizeof(int) );
  nbngb_loc = malloc( ldnbngb_loc * ldnbngb_loc * sizeof(int) );

  
  if (rank == root)
    printf("Starting number of living cells = %d\n", num_alive);
  
  // AllReduce
  t1 = mytimer();
  for (loop = 1; loop <= maxloop; loop++) {
    

    cell_loc(   0, 0   ) = cell_loc((BS/size), (BS/size));
    cell_loc(   0, (BS/size)+1) = cell_loc((BS/size),  1);
    cell_loc((BS/size)+1, 0   ) = cell_loc( 1, (BS/size));
    cell_loc((BS/size)+1, (BS/size)+1) = cell_loc( 1,  1);

    
    for (i = 1; i <= (BS/size); i++) {
      cell_loc(   i,    0) = cell_loc( i, (BS/size));
      cell_loc(   i, (BS/size)+1) = cell_loc( i,  1);
      cell_loc(   0,    i) = cell_loc((BS/size),  i);
      cell_loc((BS/size)+1,    i) = cell_loc( 1,  i);
    }


    for (j = 1; j <= BS/size; j++) {
      for (i = 1; i <= BS/size; i++) {
	ngb_loc( i, j ) =
	  cell_loc( i-1, j-1 ) + cell_loc( i, j-1 ) + cell_loc( i+1, j-1 ) +
	  cell_loc( i-1, j   ) +                  cell_loc( i+1, j   ) +
	  cell_loc( i-1, j+1 ) + cell_loc( i, j+1 ) + cell_loc( i+1, j+1 );
      }
    }

    num_alive = 0;
    for (j = 1; j <= BS/size; j++) {
      for (i = 1; i <= BS/size; i++) {
	if ( (ngb_loc( i, j ) < 2) ||
	     (ngb_loc( i, j ) > 3) ) {
	  cell_loc(i, j) = 0;
	}
	else {
	  if ((ngb_loc( i, j )) == 3)
	    cell_loc(i, j) = 1;
	}
	if (cell_loc(i, j) == 1) {
	  num_alive ++;
	}
      }
    }

    /* Avec les celluls sur les bords (utile pour vérifier les comm MPI) */
    /* output_board( BS+2, &(cell(0, 0)), ldboard, loop ); */

    /* Avec juste les "vraies" cellules: on commence à l'élément (1,1) */
    /*output_board( BS, &(cell(1, 1)), ldboard, loop);*/

    printf("%d cells are alive\n", num_alive);
  }

  t2 = mytimer();
  temps_loc = t2 - t1;
  if (rank == root){
    printf("Final number of living cells = %d\n", num_alive);
    printf("%.2lf\n",(double)temps * 1.e3);
  }
  
  free(board);
  free(nbngb);
  free(board_loc);
  free(nbngb_loc);
  return EXIT_SUCCESS;
}

