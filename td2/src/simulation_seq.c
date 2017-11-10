#include <simulation_seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <particule.h>
#include <util.h>

#define NB_ITERATIONS 2000

int main(int argc, char** argv){
  int m; char ligne[MAX];
  if (argc != 3) {
    printf("Erreur : Argument manquant\n");
    printf("Usage : ./exec particules_file output_file\n");
    return EXIT_FAILURE;
  }
  /* Fichier de particules */
  FILE* fd =fopen(argv[1], "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    return EXIT_FAILURE;
  } 
  FILE* output = fopen(argv[2], "w+");
  if (output == NULL){
    perror("fopen : fichier output\n");
    return EXIT_FAILURE;
  } 
  fgets(ligne, MAX, fd);
  m = atoi(ligne);
  fclose(fd);
  fflush(stdout);
  int k;
  int i = 0; int n = 0; int p = 0;
  double dt = 0.0, t = 0.0; 
  particule univers[m];
  parse_particules(argv[1], univers);
  /*
  for(int j=0; j<m; ++j)
    print_particule(univers+j);
  */
  vecteur force_tmp;
  while (i < NB_ITERATIONS){
    // calcul des forces exterieures
    while (n < m){
      while (p < m){
	if (n == p){
	  p++;
	  continue;
	}	
	// calcul de la distance de la particule la plus proche
	double dist = distance(&univers[p], &univers[n]);
	//printf("distance : %lf\n", dist);
	if (univers[n].proche_d == 0.0 || 
	    dist < univers[n].proche_d){
	  univers[n].proche_d = dist; 
	}
	force_grav(&univers[p], &univers[n], &force_tmp);
	somme(&(univers[n].f_ext),&(force_tmp),&(univers[n].f_ext));
	p++;
      }
      p=0;
      n++;
    }
    //puts("############ DEBUT ###############");
    n = 0;
    dt = nouveau_dt(univers, m);
    t += dt;
    fprintf(output, "%lf\n", t);
    // mise à jour des particules
    update_particules(univers, m, dt);
    log_particules(univers, m, output);
    i++; 
    //puts("############ FIN ###############");
  }
  fclose(output);
  return 0;
}
