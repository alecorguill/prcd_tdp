#include <simulation_seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <particule.h>
#include <util.h>

#define NB_ITERATIONS 1e6
#define MAX 10 //taile decimale du nombre de particule

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
      univers[n].proche_d = 0.0;
      while (p < m){
	if (n == p){
	  p++;
	  continue;
	}

	// calcul de la distance de la particule la plus proche
	double dist = distance(&univers[p], &univers[n]);
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
    /*
    puts("############ DEBUT ###############");
    print_particule(univers);
    print_particule(univers+1);
    */
    n = 0;
    dt = nouveau_dt(univers, m);
    t += dt;
    fprintf(output, "%lf\n", t);
    // mise à jour des particules
    while (n < m){	
      update_acceleration(&univers[n]);
      update_position(&univers[n], dt);	    
      update_vitesse(&univers[n], dt);
      fprintf(output, "%lf,%lf\n", univers[n].p.x, univers[n].p.y);
      n++;
    }
    i++; 
    //puts("############ FIN ###############");
  }
  fclose(output);
  return 0;
}
