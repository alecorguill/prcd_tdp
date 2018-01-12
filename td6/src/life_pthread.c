#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include <omp.h>
int BS;
int num_thread;
pthread_mutex_t locks[num_threads];

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
  num_threads = atoi(getenv("MY_NUM_THREADS"));
  pthread_t thread_id[NTHREADS];
  for(int i=0;i<NTHREADS;i++)
    {
      pthread_create( &thread_id[i], NULL, init_board, NULL );
    }
  

  
  num_alive = 0;

  /* Leading dimension of the board array */
  ldboard = BS + 2;
  /* Leading dimension of the neigbour counters array */
  ldnbngb = BS;

  board = malloc( ldboard * ldboard * sizeof(int) );
  nbngb = malloc( ldnbngb * ldnbngb * sizeof(int) );

  num_alive = generate_initial_board( BS, &(cell(1, 1)), ldboard );

  printf("Starting number of living cells = %d\n", num_alive);
  t1 = mytimer();

  for (loop = 1; loop <= maxloop; loop++) {
    init_board(board);
    count_ngbs(ngbs,nbngb);
    update_cells(board,lock);
    

    /* Avec les celluls sur les bords (utile pour vérifier les comm MPI) */
    /* output_board( BS+2, &(cell(0, 0)), ldboard, loop ); */

    /* Avec juste les "vraies" cellules: on commence à l'élément (1,1) */
    /*output_board( BS, &(cell(1, 1)), ldboard, loop);*/

    printf("%d cells are alive\n", num_alive);
  }

  t2 = mytimer();
  temps = t2 - t1;
  printf("Final number of living cells = %d\n", num_alive);
  printf("%.2lf\n",(double)temps * 1.e3);

  free(board);
  free(nbngb);
  return EXIT_SUCCESS;
}

