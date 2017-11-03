#ifndef UTIL_H
#define UTIL_H

void parse_particules(char *filename, particule *ps);
double nouveau_dt(particule* univers, int m);
int next_int(FILE * f);
#endif