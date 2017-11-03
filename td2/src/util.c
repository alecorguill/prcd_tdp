#include <util.h>
#include <particule.h>
#include <math.h>

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


double solution_equ(particule);

void parse_particules(char *filename, particule *ps){
  FILE *f = NULL;
  char ligne[TAILLE_LIGNE];
  f=fopen(filename, "r");
  fclose(f);
  while ( fgets(ligne, TAILLE_LIGNE, fichier) != NULL ){
    ps->m = next_int(f);
    ps->p.x = next_int(f);
    ps->p.y = next_int(f);
    ps->v.x = next_int(f);
    ps->v.y = next_int(f);
    ++ps;
  }
}

/* 
 * renvoie le prochain entier sur une ligne ou des entiers sont séparés
 * par des espaces.
 */
int next_int(FILE * f){
  int res = fgetc(f);
  fgetc(f);
  return res;
}

int double solution_equ(particule p){
  double a = norme(p.a) / 2;
  double b = norme(p.v) / 2;
  double c = 0.1 * p.proche_d;
  double delta = b*b - 4*a*c;
  double x;

  x = (-b + sqrt(abs(delta))) / 2 * a;
  return x;
}

