#ifndef BLAS_H
#define BLAS_H

#include "cblas.h"

void dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
                 const int K, const double alpha, const double *A,
                 const int lda, const double *B, const int ldb,
                 const double beta, double *C, const int ldc);


void cblas_dgemm_scalaire_gen(const int m, const int n, const int K, 
			      const double *A,const int lda, const double *B,
			      const int ldb, double *C, const int ldc);

void cblas_dgemm_scalaire(const int m, const double *A,const int lda, 
			  const double *B, const int ldb,
			  double *C, const int ldc);

void cblas_dgemm_scalaire_kij(const int m, const double *A,const int lda, 
			  const double *B, const int ldb,
			      double *C, const int ldc);


void cblas_dgemm_scalaire_jik(const int m, const double *A,const int lda, 
			  const double *B, const int ldb,
			      double *C, const int ldc);


#endif
