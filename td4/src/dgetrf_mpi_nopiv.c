#include <cblas.h>
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include <dgetf2_nopiv.h>
#include <mpi.h>
#include <stdio.h>

void dgetrf_mpi_nopiv(const enum CBLAS_ORDER order, const int M, const int N, double *A,
		   const int lda){
  int i;
  int root = 0;
  MPI_Init(NULL,NULL);
  // Defined like this in LAPACK
  int nb = 2;
  int size = atoi(getenv("MPI_NUM_PROCS"));
  int nb_block = N/nb;
  MPI_Comm slaves;
  if ((N%nb != 0) && (M%nb)){
    printf("Usage: Provide a matrix with a dimension that could be divised by the block size\n");
    MPI_Finalize();
    exit(-1);
  }
  int proc_nb_block = nb_block/size;
  if (nb_block%size != 0){
    printf("Number of blocks should be divisible by np\n");
    MPI_Finalize();
    exit(-1);
  }
  
  int err[size];
  MPI_Comm_spawn("./slave_lu",NULL,size,MPI_INFO_NULL,0,MPI_COMM_WORLD,&slaves, err);
  for (i = 0; i < size; i++){
    MPI_Send(&N,1,MPI_INT,i,99,slaves);
    MPI_Send(&M,1,MPI_INT,i,99,slaves);
    MPI_Send(&nb,1,MPI_INT,i,99,slaves);
  }

  int sendcounts[size],displs[size];  
  for(int i=0; i<size;++i){
    sendcounts[i] = M*(N/size);
    displs[i] = sendcounts[i]*i*sizeof(double);
  }
  MPI_Scatterv(A,sendcounts,displs,MPI_DOUBLE,NULL,M*(N/size),MPI_DOUBLE,root,slaves);
  /* MPI_Gather(); */
  MPI_Finalize();
}
