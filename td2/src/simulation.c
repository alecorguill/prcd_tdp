#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char** argv){
  if (argc != 2) 
    int m = atoi(argv[1]);
  else
    return EXIT_FAILURE;
 
  //
  MPI_Init(NULL,NULL);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int root = 0;
  if (rank == root)
    printf("Je suis le processeur root\n");
    
  MPI_Finalize();
  

  //
  return 0;
}
