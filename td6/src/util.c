#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "life.h"
#include "util.h"

#define TEST {printf("TEST %d\n",__LINE__);}

int barrier;
int barrier_id;
pthread_cond_t cond_barrier = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut_barrier = PTHREAD_MUTEX_INITIALIZER;


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
      if ( cell( i, j, board, ldboard ) == 1)
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
	cell(i, j, board, ldboard) = 1;
	num_alive ++;
      }
      else {
	cell(i, j, board, ldboard) = 0;
      }
      //printf("%d ",cell(i, j, board, ldboard));
    }
    //printf("\n");
  }

  return num_alive;
}


int get_thread_index(game *g){
  pthread_t self = pthread_self();
  for(int i=0;i<g->num_threads;++i){
    if(g->thread_ids[i]==self)
      return i;
  }
  return -1;
}

void barrier_STOP(int nb_threads){
    pthread_mutex_lock(&mut_barrier);
    barrier++;

    if (barrier == nb_threads) {
	barrier = 0;
	pthread_cond_broadcast(&cond_barrier);
    }
    else{
	pthread_cond_wait(&cond_barrier, &mut_barrier);
    }
    pthread_mutex_unlock(&mut_barrier);
}


void *start_game(void * ga){
  int index;
  game *g;
 
  g = (game *) ga;
  index = get_thread_index(g);
  if(index == -1){
    fprintf(stderr,"Problem thread\n");
    exit(EXIT_FAILURE);
  }
  if(index==0)
    pthread_init_board(g);
  barrier_STOP(g->num_threads);
  for (int loop = 1; loop <= g->maxloop; loop++) {
    /* upate edges */
    if(index == 0){
      cell(0,0,g->board,g->ldboard) = cell(g->BS,g->BS,g->board,
					   g->ldboard);
      cell(0,g->BS+1,g->board,g->ldboard) = cell(g->BS,1,g->board,
						 g->ldboard);
      cell(g->BS+1,0,g->board,g->ldboard) = cell(1,g->BS,g->board,
						 g->ldboard);
      cell(g->BS+1,g->BS+1,g->board,g->ldboard) = cell(1,1,g->board,
						       g->ldboard);
      
      for (int i = 1; i <= g->BS; i++) {
	cell(i,0,g->board,g->ldboard) = cell(i,g->BS,g->board,
					     g->ldboard);
	cell(i,g->BS+1,g->board,g->ldboard) = cell(i,1,g->board,
						   g->ldboard);
	cell(0,i,g->board,g->ldboard) = cell(g->BS,i,g->board,
					     g->ldboard);
	cell(g->BS+1,i,g->board,g->ldboard) = cell(1,i,g->board,
						   g->ldboard);
      }
    }
    barrier_STOP(g->num_threads);
    /* ------ */
    /* neighbour compute */
    int jbegin,jend;
    jbegin = index*(g->BS/g->num_threads)+1;
    jend = (index+1)*(g->BS/g->num_threads);
    /* neighbour compute on the sides */
    /* left side */
    for(int i=1;i<=g->BS;++i){
      ngb(i,jbegin,g->nbngb,g->ldnbngb)=
	cell(i-1,jbegin-1,g->board,g->ldboard)+
	cell(i,jbegin-1,g->board,g->ldboard)+
	cell(i+1,jbegin-1,g->board,g->ldboard)+
	cell(i-1,jbegin,g->board,g->ldboard)+
	cell(i+1,jbegin,g->board,g->ldboard)+
	cell(i-1,jbegin+1,g->board,g->ldboard)+
	cell(i,jbegin+1,g->board,g->ldboard)+
	cell(i+1,jbegin+1,g->board,g->ldboard);
    }
    if(index != 0){
      sem_post(&(g->sems[index-1].sem));
      pthread_cond_signal(&(g->sems[index-1].cond));
      //printf("Je suis %d je signal %d\n",index,index-1);
    }
    /* right side */
    for(int i=1;i<=g->BS;++i){
      ngb(i,jend,g->nbngb,g->ldnbngb)=
	cell(i-1,jend-1,g->board,g->ldboard)+
	cell(i,jend-1,g->board,g->ldboard)+
	cell(i+1,jend-1,g->board,g->ldboard)+
	cell(i-1,jend,g->board,g->ldboard)+
	cell(i+1,jend,g->board,g->ldboard)+
	cell(i-1,jend+1,g->board,g->ldboard)+
	cell(i,jend+1,g->board,g->ldboard)+
	cell(i+1,jend+1,g->board,g->ldboard);
    }
    if(index != (g->num_threads-1)){
      sem_post(&(g->sems[index+1].sem));
      pthread_cond_signal(&(g->sems[index+1].cond));
      //printf("Je suis %d je signal %d\n",index,index+1);
    }
    /* neighbour compute center cells */
    for(int j=jbegin+1;j<jend;++j){
      for(int i=1;i<=g->BS;++i){
	ngb(i,j,g->nbngb,g->ldnbngb)=
	  cell(i-1,j-1,g->board,g->ldboard)+
	  cell(i,j-1,g->board,g->ldboard)+
	  cell(i+1,j-1,g->board,g->ldboard)+
	  cell(i-1,j,g->board,g->ldboard)+
	  cell(i+1,j,g->board,g->ldboard)+
	  cell(i-1,j+1,g->board,g->ldboard)+
	  cell(i,j+1,g->board,g->ldboard)+
	  cell(i+1,j+1,g->board,g->ldboard);
      }
    }
    /* g->num_alive=0; */
    /* update center cells */
    for(int j=jbegin+1;j<jend;++j){
      for(int i=1;i<=g->BS;++i){
	if ( (ngb(i,j,g->nbngb,g->ldnbngb) < 2) ||
    	     (ngb(i,j,g->nbngb,g->ldnbngb) > 3) ) {
  	  cell(i,j,g->board,g->ldboard) = 0;
  	}
  	else {
  	  if ((ngb(i,j,g->nbngb,g->ldnbngb)) == 3)
  	    cell(i,j,g->board,g->ldboard) = 1;
  	}
  	/* if (cell(i,j,g->board,g->ldboard) == 1) { */
  	/*   g->num_alive ++; */
  	/* } */
      }
    }
    
    /* checking if neighbour read border cells */
    int count,value,bool=0;
    count = (index==0 || index==(g->num_threads-1)) ? 1 : 2;
    sem_getvalue(&(g->sems[index].sem),&value);
    while(value != count){
      pthread_cond_wait(&(g->sems[index].cond),
			&(g->sems[index].m));
      sem_getvalue(&(g->sems[index].sem),&value);
    }
    /* reset semaphores */
    for(int i=0;i<count;++i)
      sem_wait(&(g->sems[index]).sem);
    /* neighbour OK we can update sides */
    /* left side */
    for(int i=1;i<=g->BS;++i){
      if ( (ngb(i,jbegin,g->nbngb,g->ldnbngb) < 2) ||
	   (ngb(i,jbegin,g->nbngb,g->ldnbngb) > 3) ) {
	cell(i,jbegin,g->board,g->ldboard) = 0;
      }
      else {
	if ((ngb(i,jbegin,g->nbngb,g->ldnbngb)) == 3)
	  cell(i,jbegin,g->board,g->ldboard) = 1;
      }
      /* if (cell(i,jbegin,g->board,g->ldboard) == 1) { */
      /* 	g->num_alive ++; */
      /* } */
    }

    /* right side */
    for(int i=1;i<=g->BS;++i){
      if ( (ngb(i,jend,g->nbngb,g->ldnbngb) < 2) ||
	   (ngb(i,jend,g->nbngb,g->ldnbngb) > 3) ) {
	cell(i,jend,g->board,g->ldboard) = 0;
      }
      else {
	if ((ngb(i,jend,g->nbngb,g->ldnbngb)) == 3)
	  cell(i,jend,g->board,g->ldboard) = 1;
      }
      /* if (cell(i,jend,g->board,g->ldboard) == 1) { */
      /* 	g->num_alive ++; */
      /* } */
    }
    barrier_STOP(g->num_threads);
    /* if(index==0) */
    /*   output_board( g->BS, &(cell(1, 1, g->board, g->ldboard)), g->ldboard, loop); */
  }
  if(index == 0){
    int num_alive=0;
    for (int j = 1; j <= g->BS; j++) {
      for (int i = 1; i <= g->BS; i++) {
	if (cell(i,j,g->board,g->ldboard) == 1) {
	  num_alive ++;
	}
      }
    }
    /* printf("Final number of living cells = %d\n", num_alive); */
  }
}

    


void pthread_init_board(game *g){
  g->num_alive = generate_initial_board( g->BS, &(cell(1, 1, g->board, g->ldboard)), g->ldboard );
  /* printf("Starting number of living cells = %d\n", g->num_alive); */
  /*  output_board( g->BS, &(cell(1, 1, g->board, g->ldboard)), g->ldboard, 0); */
}
			
