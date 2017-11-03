#include <stdio.h>
#include <string.h>
#include <particule.h>

#define NB_ITERATIONS 10

int main(int argc, char** argv){
  if (argc != 2) {
    int m = atoi(argv[1]);
  }
  else
    return EXIT_FAILURE;
  //
  int i = 0; int n = 0; int p = 0;
  double T = 5.0;
  double dt = 0.0; // dt fixe
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
	  univers[n].proche_d = distance(univers[p], univers[n];

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
