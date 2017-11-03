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
  int nb_ligne=0;
  fgets(ligne, TAILLE_LIGNE, fichier);
  int nb_particule = ligne
  while ( fgets(ligne, TAILLE_LIGNE, fichier) != NULL ){
    nb_ligne
  }
    printf("%s", ligne ) ;
}

double solution_equ(particule p){
  double a = norme(p.a) / 2;
  double b = norme(p.v) / 2;
  double c = 0.1 * p.proche_d;
  double eps = 0.001;
  double delta = b*b - 4*a*c;
  double x;
  
  if (abs(delta) < eps){
    x = -b / 2 * (a);
    return x;
  }   
  else {
    x = (-b + sqrt(abs(delta))) / 2 * a;
    return x;
  }
}

