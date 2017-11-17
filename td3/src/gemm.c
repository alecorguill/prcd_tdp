#include <gemm.h>

void gemm(const int m, const int n, const int K, const double *A,const int lda, const double *B, const int ldb, double *C, const int ldc){
  init_matrice(C,m,n,ldc,0.0);
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      for (int k =0; k < K; k++)
	C[j*ldc +i] += *(A+i*lda+k) * *(B+j*ldb+k);
    }
  } 
}
