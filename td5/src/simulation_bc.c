#include <simulation_seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <particule.h>
#include <util.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NB_ITERATIONS 20

int main(int argc, char** argv){
  int size; char ligne[MAX];
  if (argc != 4) {
    printf("Erreur : Argument manquant\n");
    printf("Usage : ./exec particules_file nb_blocs output_file\n");
    return EXIT_FAILURE;
  }
  /* Fichier de particules */
  FILE* fd =fopen(argv[1], "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    return EXIT_FAILURE;
  } 
  /* Fichier d'output des résultats */
  int output = open(argv[3], O_CREAT | O_RDWR, 0755);
  if (output == -1){
    perror("open : fichier output\n");
    return EXIT_FAILURE;
  } 

  fgets(ligne, MAX, fd);
  size = atoi(ligne);
  fclose(fd);
  fflush(stdout);
	
  int k;
  int i = 0, n = 0, p = 0;
  double dt = 0.0, t = 0.0; 

  /* Recuperation des particules à partir du fichier */
  int nb_blocs = atoi(argv[2]);
  int dims[nb_blocs];	
  bloc univers[nb_blocs];
  parse_particules_bloc(argv[1], univers);
  
  /* Calcul des centres de masses */
  for (int ii = 0; ii < nb_blocs; ii++){
    masse_center(&univers[ii]);
    print_particule(&(univers[ii].center));
  }
  
  while (i < NB_ITERATIONS){
    // calcul des forces exterieures
    while (n < nb_blocs){
      while (p < nb_blocs){

  	// calcul de la distance de la particule la plus proche (pour dt)
	process_interaction_bloc(&univers[n],&univers[p]);    
	p=0;
	n++;
      }

      n = 0;
      // TODO
      /* dt = nouveau_dt(univers, m); */
      /* t += dt; */
      /* /\* log de dt *\/ */
      /* dprintf(output, "%lf\n", t); */
      /* /\* mise à jour des particules *\/ */
      /* // TODO */
      /* update_particules(univers, m, dt); */
      /* /\* log des particules *\/ */
      /* // TODO */
      /* log_particules(univers, m, output); */
      i++;
    }
    /* for(int ii=0; ii<nb_blocs; ++ii){ */
    /*   free(univers[ii].ps); */
  }
  close(output);
  return 0;
}