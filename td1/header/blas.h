#ifndef BLAS_H
#define BLAS_H


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