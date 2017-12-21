#include <cblas.h>
#include "util.h"
#include <dgetf2_nopiv.h>
#include <stdio.h>

void dgetrf_nopiv(const enum CBLAS_ORDER order, const int M, const int N, double *A,
		   const int lda){
  int j,jb; double scal;
  // Defined like this in LAPACK
  int nb = 64;
  if (nb < MIN(N,M))
    for (j=0; j<MIN(N,M); j+=nb){
      jb = MIN(MIN(M,N)-j,nb);
      dgetf2_nopiv(CblasColMajor,M-j,jb,COEF(A,j,j,lda),lda);
      if ( j + jb < N){
	dtrsm('L','L','N','U',jb,N-j-jb,1,COEF(A,j,j,lda),lda,COEF(A,j,j+jb,lda),lda);
	if ( j + jb < M)
	  dgemm(M-j-jb,N-j-jb,jb,-1,COEF(A,j+jb,j,lda),lda,COEF(A,j,j+jb,lda),lda,COEF(A,j+jb,j+jb,lda),lda);   
      }     
    }
  else
    dgetf2_nopiv(CblasColMajor,M,N,A,lda);
}
