#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "util.h"
#include "blas.h"
#define NUM_THREADS 5
#define TB 2

void dgemm_parallel(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
	   const enum CBLAS_TRANSPOSE TransB, const int M, const int N, 
	   const int K, const double alpha,
	   const double *A, const int lda, const double *B, const int ldb,
	   const double beta, double *C, const int ldc)
{
  char* var = getenv("MYLIB_NUM_THREADS");
  int MY_LIB_NUM_THREADS=1;
  if(var != NULL)
    MY_LIB_NUM_THREADS=atoi(var);
  else
    MY_LIB_NUM_THREADS = NUM_THREADS;
  omp_set_num_threads(MY_LIB_NUM_THREADS);
  //
  init_matrice(C,M,M,ldc,0.0);
  int nb_l = M/TB;
  int nb_c = N/TB;
  int n,p; double* tmp;
    
    for (int i = 0; i < nb_l; i++){
#pragma omp parallel for nowait
    for (int j = 0; j < nb_c; j++){
      
      p = (j == nb_c-1) ? j%TB : TB;
      
      for (int k =0; k < nb_l; k++){
	n = (k == nb_l-1) ? k%TB : TB;
	tmp = malloc(sizeof(double)*n*p);
	cblas_dgemm_scalaire(n,A+i*lda*TB+k*TB, M, B+j*ldb*TB+k*TB, M, tmp,n);
	
	printf("i, j, k  : %d, %d, %d / %d-%d\n", i,j,k,n,p);
	affiche(n,p,tmp,n,1);
	somme_matrice(n,p,C+j*ldc*TB+i*TB, ldc, tmp, TB, C+j*ldc*TB+i*TB, ldc);
	free(tmp);
	
	}
      // cas du reste

    }
  }
}


/* void dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, */
/* 	   const enum CBLAS_TRANSPOSE TransB, const int M, const int N,  */
/* 	   const int K, const double alpha, */
/* 	   const double *A, const int lda, const double *B, const int ldb, */
/* 	   const double beta, double *C, const int ldc) */
/* { */
/*   init_matrice(C,M,M,ldc,0.0); */
/*   if(TransA != CblasTrans){ */
/*     //Transposer la matrice */
/*   } */
/*   else { */
/*     // taille des blocs  */
/*     int tb = 2; */
/*     // nombre de blocs */
/*     int nb = M/tb; */
/*     for (int i = 0; i < nb; i++){ */
/*       for (int j = 0; j < nb; j++){ */
/* 	double* tmp = malloc(sizeof(double)*tb*tb); */
/* 	init_matrice(tmp,tb,tb,tb,0.0); */
/* 	for (int k =0; k < nb; k++){ */
/* 	  cblas_dgemm_scalaire(tb, A+i*lda*tb+k*tb, M, B+j*ldb*tb+k*tb, M, tmp,tb); */
/* 	  somme_matrice(tb,tb,C+j*ldc*tb+i*tb, ldc, tmp, tb, C+j*ldc*tb+i*tb, ldc); */
/* 	} */
/* 	free(tmp); */
/*       } */
/*     } */
/*   } */
/* } */


/* cblas_dgemm_scalaire_ijk non carré */
void cblas_dgemm_scalaire_gen(const int m, const int n, const int K, const double *A,const int lda, const double *B, const int ldb, double *C, const int ldc){
  init_matrice(C,m,n,ldc,0.0);
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      for (int k =0; k < K; k++)
	C[j*ldc +i] += *(A+i*lda+k) * *(B+j*ldb+k);
    }
  } 
}

/*
  Produit matriciel de matrices carrées A'.B (A' signifie transposé de A)
  m : dimension
  A : matrice A
  lda : leading dimension de A
  B : matrice B
  ldb : leading dimension de B
  C : matrice resultante
  ldc : leading dimension de C
*/

/* cblas_dgemm_scalaire_ijk */
void cblas_dgemm_scalaire(const int m, const double *A,const int lda, 
			  const double *B, const int ldb,
			  double *C, const int ldc){
  init_matrice(C,m,m,ldc,0.0);
  for (int i = 0; i < m; i++){
    for (int j = 0; j < m; j++){
      for (int k =0; k < m; k++)
	C[j*ldc +i] += *(A+i*lda+k) * *(B+j*ldb+k);
    }
  } 
}

void cblas_dgemm_scalaire_kij(const int m, const double *A,const int lda, 
			  const double *B, const int ldb,
			  double *C, const int ldc){
  
  init_matrice(C,m,m,ldc,0.0);
  for (int k = 0; k < m; k++){
    for (int i = 0; i < m; i++){
      for (int j =0; j < m; j++)
	C[j*ldc +i] += *(A+i*lda+k) * *(B+j*ldb+k);
    } 
  }
}


void cblas_dgemm_scalaire_jik(const int m, const double *A,const int lda, 
			      const double *B, const int ldb,
			      double *C, const int ldc){
  
  init_matrice(C,m,m,ldc,0.0);
  for (int j = 0; j < m; j++){
    for (int i = 0; i < m; i++){
      for (int k =0; k < m; k++){
	C[j*ldc +i] += *(A+i*lda+k) * *(B+j*ldb+k);
      } 
    }
  }
}
