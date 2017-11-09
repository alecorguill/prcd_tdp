#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include "particule.h"

#define EPS 1E-6

int equal_double(double a, double b);
double solution_equ(particule p);
void parse_particules(char *filename, particule *ps);
double nouveau_dt(particule* univers, int m);

#endif
