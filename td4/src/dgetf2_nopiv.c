#include <cblas.h>
#include "util.h"
#include <stdio.h>


// Works only on squared matrix
void dgetf2_nopiv(const enum CBLAS_ORDER order, const int M, const int N, double *A,
		   const int lda){
  int j; double scal;
  for (j=0; j<N; j++){
    scal = 1.0/(*COEF(A,j,j,lda));
    dscal(M-j-1,scal,COEF(A,(j+1),j,lda),1);    
    dger(CblasColMajor,M-j-1,N-j-1,-1,COEF(A,(j+1),j,lda),1, COEF(A,j,(j+1),lda),lda,COEF(A,(j+1),(j+1),lda),lda);
  }
}
