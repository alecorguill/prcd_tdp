#include <stdlib.h>
#include <stdio.h>
#include <ddot.h>

/* y := alpha*x + y */
void daxpy(const int N, const double alpha, const double *X,
	   const int incX, double *Y, const int incY){
  
  int i = 0;
  while (i < N){
    *(Y+incY+i) = *(Y+incY+i) + alpha * *(X+incX+i);
    i++;
  }

}

/* y := alpha*A'*x + beta*y*/
void dgemv(const int M, const int N, const double alpha, 
	   const double *A, const int lda, const double *X, 
	   const int incX, const double beta, double *Y, const int incY){

  int i = 0;
  while (i < N){
    *(Y+incY+i) = alpha * ddot(M,A,lda,X,incX) + beta * *(Y+incY+i);
    i++;
  }
  
}

/* C := C + alpha*A * B  */
void dgemm(const int M, const int N,
	   const double alpha, const double *A,
	   const int lda, const double *B, const int ldb,
	   double *C, const int ldc){
   for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      *(C+ldc*i+j) = *(C+ldc*i+j) + alpha * *(A+lda*i+j) * *(B+ldb*i+j);
    }
  }
}

/* A := alpha*x*y' + A*/
void dger(const int M, const int N, const double alpha, const double *X, 
	  const int incX,const double *Y, const int incY, double *A, const int lda){
  for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      *(A+lda*i+j) = *(A+lda*i+j) + alpha * *(X+incX*i) * *(Y+incY+j);
    }
  }

}

/* void saxpy(const int N, const float alpha, const float *X, */
/*                  const int incX, float *Y, const int incY); */

/* void sgemv(const int M, const int N, */
/* 	   const float alpha, const float *A, const int lda, */
/* 	   const float *X, const int incX, const float beta, */
/* 	   float *Y, const int incY); */

/* void sgemm(const int M, const int N, */
/* 	   const int K, const float alpha, const float *A, */
/* 	   const int lda, const float *B, const int ldb, */
/* 	   const float beta, float *C, const int ldc); */

/* void sger(const int M, const int N, */
/* 	  const float alpha, const float *X, const int incX, */
/* 	  const float *Y, const int incY, float *A, const int lda); */
