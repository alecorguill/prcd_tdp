#include "dgemm.h"
#include "util.h"
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


/* Produit matriciel par bloc */
/* void cblas_dgemm(const int m, const double *A,const int lda, */
/* 		 const double *B, const int ldb, */
/* 		 const double *C, const int ldc){ */
  
/*   init_matrice(C,m,m,ldc,0.0); */
/*   int m1,m2; */
/* } */
