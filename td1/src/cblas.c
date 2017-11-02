#include "cblas.h"
#include "util.h"




/* y := alpha*x + y */
void cblas_daxpy(const int N, const double alpha, const double *X,
	   const int incX, double *Y, const int incY){
  
  int i = 0;
  while (i < N){
    *(Y+incY+i) = *(Y+incY+i) + alpha * *(X+incX+i);
    i++;
  }

}

/* y := alpha*A'*x + beta*y*/
void cblas_dgemv(const enum CBLAS_ORDER order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const double alpha, 
	   const double *A, const int lda, const double *X, 
	   const int incX, const double beta, double *Y, const int incY){

  int i = 0;
  while (i < N){
    *(Y+incY+i) = alpha * cblas_ddot(M,A+i*lda,1,X,incX) + beta * *(Y+incY+i);
    i++;
  }
  
}

/* C := C + alpha*A * B  */
void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
		 const double alpha, const double *A,
		 const int lda, const double *B, const int ldb,
		 const double beta, double *C, const int ldc){
   for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      *(C+ldc*i+j) = *(C+ldc*i+j) + alpha * *(A+lda*i+j) * *(B+ldb*i+j);
    }
  }
}

/* A := alpha*x*y' + A*/
void cblas_dger(const enum CBLAS_ORDER order, const int M, const int N, const double alpha, const double *X, 
	  const int incX,const double *Y, const int incY, double *A, const int lda){
  for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      *(A+lda*i+j) = *(A+lda*i+j) + alpha * *(X+incX+i) * *(Y+incY+j);
    }
  }

}


/*
  Produit scalaire X.Y
  m : dimension
  dx : vecteur X
  incx : decalage pour le premier element du produit sur X
  dy : vecteur y
  incy : decalage du premier element du produit sur Y
 */
double cblas_ddot(const int m, const double *dx, const int incx, 
	    const double *dy, const int incy){
  double resultat = 0.0;
  int i = 0;
  while (i < m){
    resultat += *(dx+incx*i) * *(dy+incy*i);
    i++;
  }
  return resultat;

}

