#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include "particule.h"
#define MAX 486 //taile decimale du nombre de particule
#define EPS 1E-6

int equal_double(double a, double b);
double solution_equ(particule p);
void parse_particules(char *filename, particule *ps);
double nouveau_dt(particule* univers, int m);
void parse_particule_par(char * filename, int rank, particule* univers);
void init_buffers(int, particule*, particule*);
#endif
