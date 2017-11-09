#include <simulation_seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <particule.h>
#include <util.h>

#define NB_ITERATIONS 10
#define MAX 10 //taile decimale du nombre de particule

int main(int argc, char** argv){
  int m; char ligne[MAX];
  if (argc != 2) {
    printf("Erreur fichier initial non spécifié\n");
    return EXIT_FAILURE;
  }
  
  FILE* fd =fopen(argv[2], "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    return EXIT_FAILURE;
  }
  fgets(ligne, MAX, fd);
  m = atoi(ligne);
  fclose(fd);
  
  //
  int i = 0; int n = 0; int p = 0;
  double dt = 0.0; 
  particule univers[m];
  vecteur force_tmp;
  while (i < NB_ITERATIONS){
    // calcul des forces exterieures
    while (n < m){
      univers[n].proche_d = 0.0;
      while (p < m){
	if (n == p)
	  continue;	

	// calcul de la distance de la particule la plus proche
	if (univers[n].proche_d == 0.0 || 
	    univers[n].proche_d > univers[p].proche_d) 
	  univers[n].proche_d = distance(&univers[p], &univers[n]);

	force_grav(&univers[p], &univers[n], &force_tmp);
	somme(&(univers[n].f_ext),&(force_tmp),&(univers[n].f_ext));
	p++;
      }
      n++;
    }
    n = 0;
    dt = nouveau_dt(univers, m);
    // mise à jour des particules
    while (n < m){	
      update_acceleration(&univers[m]);
      update_position(&univers[m], dt);	    
      update_vitesse(&univers[m], dt);
    }
    i++;
  }
  //
  return 0;
}
