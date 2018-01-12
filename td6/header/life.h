#ifndef LIFE_H
#define LIFE_H

#define cell( _i_, _j_ ) board[ ldboard * (_j_) + (_i_) ]
#define ngb( _i_, _j_ )  nbngb[ ldnbngb * ((_j_) - 1) + ((_i_) - 1 ) ]

typedef struct {
  int * data;
  pthread_mutex_t lock;
}locked_int

void *init_board(void *board);
void count_ngbs(int *board, int *nbngb);
void update_cells(int *board, locked_int *num_alive,
		  pthread_mutex_t *locks);

#endif
