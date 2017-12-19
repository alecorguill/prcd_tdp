#include <cblas.h>
#include "util.h"
#include <dgetf2_nopiv.h>

void dgetrf_nopiv(const enum CBLAS_ORDER order, const int M, const int N, double *A,
		   const int lda){
  int j,jb; double scal;
  // Defined like this in LAPACK
  int nb = 64;
  if (nb < MIN(N,M))
    for (j=0; j<MIN(N,M); j+=nb){
      jb = MIN(MIN(M,N)-j+1,nb);
      dgetf2_nopiv(CblasColMajor,M-j+1,jb,COEF(A,j,j,lda),lda);
      dtrsm('L','L','N','U',jb,N-j-jb+1,1,COEF(A,j,j,lda),lda,COEF(A,j,j+jb,lda),lda);
      if ( j + jb < M)
	dgemm(M-j-jb+1,N-j-jb+1,jb,-1,COEF(A,j+jb,j,lda),lda,COEF(A,j,j+jb,lda),lda,COEF(A,j+jb,j+jb,lda),lda);   
  }
  else
    dgetf2_nopiv(CblasColMajor,M,N,A,lda);
}
