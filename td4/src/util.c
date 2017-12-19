#include <cblas.h>
#include <util.h>
#include <stdio.h>

#define COEF(M,i,j,ldm) (M+j*ldm+i)

void dscal(const int N, const double alpha, double *X, const int incX){
  int i;
  for (i = 0;i < N;i++){
    *(X+incX*i) *= alpha;
  }
}

void daxpy(const int N, const double alpha, const double *X,
	   const int incX, double *Y, const int incY){
  
  int i = 0;
  while (i < N){
    *(Y+incY*i) = *(Y+incY*i) + alpha * *(X+incX*i);
    i++;
  }
}

void dger(const enum CBLAS_ORDER order, const int M, const int N, const double alpha, const double *X, const int incX,const double *Y, const int incY, double *A, const int lda){
  for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      *(A+lda*j+i) = *(A+lda*j+i) + alpha * *(X+incX*i) * *(Y+incY*j);
    }
  }

}
// A : lower triangular matrix
// B := alpha*inv( A )*B
void dtrsm(char side, char uplo, char transa, char diag, int M, int N, double alpha, double *A, int lda, double *B,int ldb){
  int i,j,k;
    for (j=0; j < N; j++){
      if (alpha != 1.0)
	dscal(M,alpha,COEF(B,0,j,ldb),1);
      for (k=0; k < M; k++){
      	//
      	if (*COEF(B,k,j,ldb) != 0.0)
      	  *COEF(B,k,j,ldb) /= *COEF(A,k,k,lda);
      	//
      	for (i=k+1; i<M; i++){
      	  *COEF(B,i,j,ldb) -= (*COEF(B,k,j,ldb) * (*COEF(A,i,k,lda)));
	}
      }
    }
}

/////////////////////////
void print(int M, int N, double* A, int lda, int fd){
  int i = 0;
  int j = 0;
  while(i < M){
    while(j < N){
      dprintf(fd, "%f ", A[j*lda+i]);
      ++j;
    }
    j=0;
    ++i;
    dprintf(fd, "\n");
  }
}
