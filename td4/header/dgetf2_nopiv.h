#ifndef DGETF_NOPIV_H
#define DGETF_NOPIV_H

void dgetf2_nopiv(const enum CBLAS_ORDER order, const int N, const int M, 
		  const double *A,const int lda);
#endif
