#include <cblas.h>
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#include "dgetf2_nopiv.h"

#include "util.h"

int main(int argc, char**argv){

  MPI_Init(NULL,NULL);
  int N,M,nb,rank,size,root=0;
  MPI_Comm parent;
  MPI_Status status;
  MPI_Comm_get_parent(&parent);
  MPI_Comm_rank(parent,&rank);
  MPI_Comm_size(parent,&size);
  MPI_Recv(&N,1,MPI_INT,0,99,parent,&status);
  MPI_Recv(&M,1,MPI_INT,0,99,parent,&status);     
  MPI_Recv(&nb,1,MPI_INT,0,99,parent,&status);     
  int nb_block = N/nb;
  double * mycol,*curcol;
  
  mycol = malloc(M*(N/size)*sizeof(double));
  curcol = malloc(M*(N/size)*sizeof(double));
  
  int sendcounts[size],displs[size];  
  for(int i=0; i<size;++i){
    sendcounts[i] = M*(N/size);
    displs[i] = sendcounts[i]*i*sizeof(double);
  }
  /* reception of local bloc */
  MPI_Scatterv(NULL,sendcounts,displs,MPI_DOUBLE,mycol,M*(N/size),MPI_DOUBLE,root,parent);
  /* start LU facto */
  for(int i=0;i<nb_block;++i){
    if( ((i*(nb_block/size)) <= rank) && (rank <  ((i+1)*(nb_block/size))) ){
      dgetf2_nopiv(CblasColMajor,M-i*nb,nb,COEF(mycol,i,i,M),M);
      /* broadcast bloc L0  pour l'inversion DTRSM */
      /* bradcast les blocs pour le dgemm */
    }
    else{
      /*
       * broadcast pour recevoir L0 pour l'inversion DTRSM
       * dtrsm();
       * broadcast reception pour dgemm
       * dgemm();
       */
    } 
  }
  /* gather();*/
 
  MPI_Finalize();
  return 0;
}
