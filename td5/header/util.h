#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include "particule.h"
#define MAX 486 //taile decimale du nombre de particule
#define EPS 1E-10 //Precision pour les opérations sur les doubles


int equal_double(double a, double b);
double solution_equ(particule p);
void parse_particules(char *filename, particule *ps);
void parse_particules_bloc(char *filename, bloc *ps);
double nouveau_dt(particule* univers, int m);
void parse_particule_par(char * filename, int rank, particule* univers);
void init_buffers(int, particule*, particule*);
void masse_center(bloc *b);
void process_interaction_bloc(bloc *a, bloc *b, float bloc_size);
void parse_par_bloc(char* filename, particule* univers);

#endif
