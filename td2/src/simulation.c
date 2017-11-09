#include <stdio.h>

#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <particule.h>

#define NB_ITERATIONS 10
#define MAX 486

void parse_particule_par(char * filename, int rank, particule** univers, int* m){
  
  int nb; char ligne[MAX];
  
  FILE* fd = fopen(filename, "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    exit(EXIT_FAILURE);
  }
  fgets(ligne, MAX, fd);
  nb = atoi(ligne);
  *m = nb;

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("size , nb, rank : %d %d %d\n", size,nb,rank);
  int alpha = nb/size;
  particule *univers_tmp = malloc(sizeof(particule)*alpha);
  
  // Lit le fichier initial 
  // Récupere les valeurs liées à son rang
  int i = 0;
  int j = 0;
  while (i < nb){
    fgets(ligne,MAX,fd);
    if (i % size == rank){
      j = i / (rank+1);
      sscanf(ligne, "%d %lf %lf %lf %lf", &((univers_tmp+i)->m),&((univers_tmp+i)->p.x),
	     &((univers_tmp+i)->p.y), &((univers_tmp+i)->v.x),&((univers_tmp+i)->v.y));
  }
    i++;
  }
  fclose(fd);
  *univers = univers_tmp;
  free(univers_tmp);
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
  // Configuration de l'univers
  /* parse_particule_par(argv[1], rank, &(univers), &m); */
 
  int nb = 100;
  int nb_rcv; 
  int tag; 
  MPI_Status status;
  MPI_Request request, request2;
  particule* buf, buf_tmp, buf_bis;
  int i = 0;
  int j = 0;
  while(i < NB_ITERATIONS) {
    while (j < size){
      MPI_Irecv(&nb, 1, MPI_INT,rank+1, tag, MPI_COMM_WORLD, &request);
      MPI_Isend(&nb, 1, MPI_INT,rank+1, tag, MPI_COMM_WORLD, &request2);
      j++;
    }
    j = 0;
    i++;
  }
  MPI_Finalize();
  //
  return 0;
}
