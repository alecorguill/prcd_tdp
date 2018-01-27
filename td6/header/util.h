#ifndef UTIL_H
#define UTIL_H

#include <pthread.h>
#include <semaphore.h>

typedef struct{
  sem_t sem;
  pthread_cond_t cond;
  pthread_mutex_t m;
}com_t;

/* structure containing data necessary for thread to run the game */
typedef struct{
  int num_threads;
  pthread_t *thread_ids;
  pthread_barrier_t *barrier;
  com_t *sems;
  int num_alive;
  int ldboard;
  int ldnbngb;
  int *board;
  int *nbngb;
  int maxloop;
  int BS;
}game;

void *start_game(void * g);
double mytimer(void);
int generate_initial_board(int N, int *board, int ldboard);
int get_thread_index(game *g);
void *start_game(void * ga);
void pthread_init_board(game *g);
double mytimer(void);
int generate_initial_board(int N, int *board, int ldboard);
#endif
