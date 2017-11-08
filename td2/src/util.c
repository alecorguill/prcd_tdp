#include <util.h>
#include <particule.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_LIGNE 81

double nouveau_dt(particule* univers, int m){
  int n = 0;
  double d_min = 0.0;
  double d;
  while (n < m){
    d = solution_equ(univers[n]); 
    if (d_min == 0.0 || d_min > d)
	d_min = d; 
    n++;
  }
  return d_min;
}


void parse_particules(char *filename, particule *ps){
  FILE *f = NULL;
  char ligne[TAILLE_LIGNE];
  f=fopen(filename, "r");
  if(f == NULL)
    perror("FOPEN");
  fgets(ligne, TAILLE_LIGNE, f);
  while ( fgets(ligne, TAILLE_LIGNE, f) != NULL ){
    char *token;
    char delim[] = " ";
    token = strtok(ligne, delim);
    ps->m = atoi(token);
    token = strtok(NULL, delim);
    ps->p.x = atof(token);
    token = strtok(NULL, delim);
    ps->p.y = atof(token);
    token = strtok(NULL, delim);
    ps->v.x = atof(token);
    token = strtok(NULL, delim);
    ps->v.y = atof(token);
    ++ps;
  }
  fclose(f);
}


double solution_equ(particule p){
  double a = norme(&p.a) / 2;
  double b = norme(&p.v) / 2;
  double c = 0.1 * p.proche_d;
  double delta = b*b - 4*a*c;
  double x;

  x = (-b + sqrt(abs(delta))) / 2 * a;
  return x;
}

