#ifndef UTIL_H
#define UTIL_H

void daxpy(const int N, const double alpha, const double *X,
	   const int incX, double *Y, const int incY);
  
void dger(const enum CBLAS_ORDER order, const int M, const int N, const double alpha, const double *X, const int incX,const double *Y, const int incY, double *A, const int lda);

void dscal(const int N, const double alpha, double *X, const int incX);

#endif // UTIL_H
