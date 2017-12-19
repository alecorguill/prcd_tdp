#ifndef UTIL_H
#define UTIL_H


void daxpy(const int N, const double alpha, const double *X,
	   const int incX, double *Y, const int incY);
  
void dger(const enum CBLAS_ORDER order, const int M, const int N, const double alpha, const double *X, const int incX,const double *Y, const int incY, double *A, const int lda);

void dscal(const int N, const double alpha, double *X, const int incX);

void print(int M, int N, double* A, int lda, int fd);

void dtrsm(char side, char uplo, char transa, char diag, int M, int N, double alpha, double *A, int lda, double *B,int ldb); 

void cblas_dgemm_lu(const int m, const int n, const double *A,const int lda, double *C, const int ldc);

double norme2(int M, int N, double* A, int lda);

void sum_matrix(const int m, const int n,double *A,const int lda, 
		  double *B, const int ldb, double *C, const int ldc);

void scal_matrix(const int m, const int n,double scal, double *A,
		 const int lda,double *C,const int ldc);

void diff_matrix(const int m, const int n,double *A,const int lda, 
		 double *B, const int ldb, double *C, const int ldc);

void random_matrix(int M, int N, int min, int max,  double* A, int lda);

#endif // UTIL_H
