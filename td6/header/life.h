#ifndef LIFE_H
#define LIFE_H

#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>



extern int BS;
/* sem_t *sems; */
/* pthread_mutex_t lock; */


void *init_board(void *board);
void count_ngbs(int *board, int *nbngb);
void update_cells(int *board);

#endif
