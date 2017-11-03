#include <util.h>
#include <particule.h>

#define TAILLE_LIGNE 81

double nouveau_dt(particule* univers, int m){
  int n = 0;
  double d_min = 0.0;
  double d;
  while (n < m){
    d = solution_equ(); 
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