#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define MAX 486

void parse_particule_par(char * filename, int rank, particule* univers, int m){
  
  int nb; char ligne[MAX];
  
  FILE* fd = fopen(filename, "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    return EXIT_FAILURE;
  }
  fgets(ligne, MAX, fd);
  nb = atoi(ligne);
  *m = nb;

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  int alpha = nb/size;
  univers = malloc(sizeof(particule)*alpha);
  
  // Lit le fichier initial 
  int i = 0;
  /* int j =  */
  /* while (i < n){ */
  /*   fgets(line,MAX,fd); */
  /*   if (i % rank == 0)  */
  /*     // remplir  */
  /*     next_double(line); */
  /* }  */
  fclose(fd);
}


int main(int argc, char** argv){
  
  if (argc != 2) {
    printf("Erreur fichier initial non spécifié\n");
    return EXIT_FAILURE;
  }
  
  MPI_Init(NULL,NULL);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int root = 0;
  particule *univers;
  int m = 0;
  parse_particule_par(argv[1], rank, univers, &m);
  
  // Configuration de l'univers
  /* if (rank == root){ */
    
  /* } */
  /* else */
     



  MPI_Finalize();
  //
  return 0;
}
