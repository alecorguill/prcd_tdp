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
  int size;char ligne[MAX];
  if (argc != 5) {
    printf("Erreur : Argument manquant\n");
    printf("Usage : ./exec particules_file nb_blocs bloc_size output_file\n");
    return EXIT_FAILURE;
  }
  /* Fichier de particules */
  FILE* fd =fopen(argv[1], "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    return EXIT_FAILURE;
  } 
  /* Fichier d'output des résultats */
  int output = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0600);
  if (output == -1){
    perror("open : fichier output\n");
    return EXIT_FAILURE;
  } 

  fgets(ligne, MAX, fd);
  size = atoi(ligne);
  fclose(fd);
  fflush(stdout);
       
  /*double dt = 0.1;*/

  /* Recuperation des particules à partir du fichier */
  int nb_blocs = atoi(argv[2]);
  float bloc_size = atof(argv[3]);
  bloc univers[nb_blocs];
  parse_particules_bloc(argv[1], univers);
  
  /* Calcul des centres de masses */
  for (int ii = 0; ii < nb_blocs; ii++){
    masse_center(&univers[ii]);
    /* for(int jj=0;jj<univers[ii].dim;++jj) */
    /*   print_particule(&(univers[ii].ps[jj])); */
  }
  
  for(int n=0;n<nb_blocs;++n){
    for(int p=0;p<nb_blocs;++p){
      process_interaction_bloc(&univers[n],&univers[p],bloc_size,(&univers[n]==&univers[p]));
    }    
  }
  
  for(int ii=0;ii<nb_blocs;++ii){
    for(int jj=0;jj<univers[ii].dim;++jj)
      dprintf(output,"%lf,%lf\n",univers[ii].ps[jj].f_ext.x,
  	     univers[ii].ps[jj].f_ext.y);
  }
  for(int ii=0; ii<nb_blocs; ++ii)
    free(univers[ii].ps);
  close(output);
  return 0;
}
