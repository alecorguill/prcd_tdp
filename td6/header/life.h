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

#define cell( _i_, _j_, board, ldboard ) board[ ldboard * (_j_) + (_i_) ]
#define ngb( _i_, _j_, nbngb, ldnbngb )  nbngb[ ldnbngb * ((_j_) - 1) + ((_i_) - 1 ) ]


#endif
