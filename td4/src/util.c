#include <cblas.h>
#include <util.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))

/*
void cblas_dgemm(const int m, const int n, const int K, const double *A,const int lda, const double *B, const int ldb, double *C, const int ldc){
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      for (int k =0; k < K; k++)
	C[j*ldc +i] += *(A+i*lda+k) * *(B+j*ldb+k);
    }
  } 
}
*/

void dscal(const int N, const double alpha, double *X, const int incX){
  int i;
  for (i = 0;i < N;i++)
    *(X+incX*i) *= alpha;
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
      *(A+lda*j+i) = (double) *(A+lda*j+i) + alpha * *(X+incX*i) * *(Y+incY*j);
    }
  }

}

void somme_matrix(const int m, const int n,double *A,const int lda, 
		   double *B, const int ldb, double *C, const int ldc)
{
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      *(C+j*ldc +i) = *(A+j*lda+i) + *(B+j*ldb+i);
    }
  } 
}

void diff_matrix(const int m, const int n,double *A,const int lda, 
		   double *B, const int ldb, double *C, const int ldc)
{
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      *(C+j*ldc +i) = *(A+j*lda+i) - *(B+j*ldb+i);
    }
  } 
}

void scal_matrix(const int m, const int n,double scal, double *A,
		 const int lda, double *C, int ldc)
{
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      *(A+j*lda +i) = scal * *(A+j*lda +i);
    }
  } 
}

void dgemm(const int m, const int n, const int K, const double *A,const int lda, const double *B, const int ldb, double *C, const int ldc){
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      for (int k =0; k < K; k++)
	C[j*ldc +i] += *(A+i+k*lda) * *(B+k+j*ldb);
    }
  } 
}


void cblas_dgemm_lu(const int m, const int n, const double *A,const int lda, double *C, const int ldc){
  if(m >= n){
    double * U = malloc(n*n*sizeof(double));
    for (int i = 0; i < n; i++){
      for (int j = i; j < n; j++){
	*(U+j*n+i)  = *(A+j*lda+i);
      }
    }
    double * L = malloc(m*n*sizeof(double));
    for (int i = 0; i < m; ++i){
      for (int j = 0; j < i+1; ++j){
	if(i == j){
	  *(L+j*m+i) = 1;
	  continue;
	}
	*(L+j*m+i)  = *(A+j*lda+i);
      }
    }
    dgemm(m,n,n,L,m,U,n,C,ldc);
    free(U);free(L);     
  }

  else{
    double * L = malloc(m*m*sizeof(double));
    for (int i = 0; i < m; ++i){
      for (int j = 0; j < i+1; ++j){
	if(i == j){
	  *(L+j*m+i) = 1;
	  continue;
	}
	*(L+j*m+i)  = *(A+j*lda+i);
      }
    }
    double * U = malloc(m*n*sizeof(double));
    for (int i = 0; i < m; i++){
      for (int j = i; j < n; j++){
	*(U+j*m+i)  = *(A+j*lda+i);
      }
    }     
    dgemm(m,n,m,L,m,U,m,C,ldc);
    free(U);free(L);         
  }
} 


void random_matrix(int M, int N, int min, int max, double* A, int lda){
  srand(time(NULL));
  int range = (max - min); 
  double div = RAND_MAX / range;      
  for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){      
      //*(A+lda*j+i) = min + (rand() / div);
      *(A+lda*j+i) = rand()%range + min;
    }
  }  
}

double norme2(int M, int N, double* A, int lda){
  double norm = 0.0;
  for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      norm += *(A+lda*j+i) * *(A+lda*j+i);
    }
  }
  return sqrt(norm);
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
