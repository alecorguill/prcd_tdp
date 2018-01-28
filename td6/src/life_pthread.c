#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <util.h>
#include <life.h>
#include <pthread.h>
#include <semaphore.h>

game g;


int main(int argc, char* argv[])
{
  double t1, t2, temps;
  
  
  if (argc < 3) {
    printf("Usage: %s nb_iterations size\n", argv[0]);
    return EXIT_SUCCESS;
  } else {
    g.maxloop = atoi(argv[1]);
    g.BS = atoi(argv[2]);
    //printf("Running sequential version, grid of size %d, %d iterations\n", BS, maxloop);
  }
  g.num_alive = 0;

  /* Leading dimension of the board array */
  g.ldboard = g.BS + 2;
  /* Leading dimension of the neigbour counters array */
  g.ldnbngb = g.BS;
  g.board = malloc( g.ldboard * g.ldboard * sizeof(int) );
  g.nbngb = malloc( g.ldnbngb * g.ldnbngb * sizeof(int) );
  for(int i=0; i<g.ldboard * g.ldboard; ++i){
    g.board[i] = 0;
  }
  /* preparing game struct */
  char* var = getenv("MY_NUM_THREADS");
  if(var == NULL){
    fprintf(stderr,"env variable MY_NUM_THREADS not set\n");
    fprintf(stderr,"export MY_NUM_THREADS=n\n");
    exit(EXIT_FAILURE);
  }
  g.num_threads = atoi(var);
  if( (g.BS % g.num_threads) != 0 ){
    fprintf(stderr,"SIZE should be divisible by MY_NUM_THREADS\n");
    exit(EXIT_FAILURE);
  }
  pthread_t thread_ids[g.num_threads];
  g.thread_ids = thread_ids;
  g.sems = malloc(g.num_threads*sizeof(com_t));
  for(int i=0;i<g.num_threads;++i){
    pthread_cond_init(&(g.sems[i]).cond,NULL);
    pthread_mutex_init(&(g.sems[i].m), NULL);
    sem_init(&(g.sems[i].sem),1,0);
  }
  /* launching threads */
  t1 = mytimer();
  for(int i=0;i<g.num_threads;i++)
    {
      pthread_create(&thread_ids[i],NULL,start_game,(void*)(&g));
    }
  /* joining threads */
  for(int i=0; i < g.num_threads; i++)
    {
      pthread_join(thread_ids[i],NULL); 
    }
  t2 = mytimer();
  temps = t2 - t1;
  printf("%.2lf\n",(double)temps * 1.e3);
  free(g.board);free(g.nbngb);free(g.sems);
  return EXIT_SUCCESS;
}

