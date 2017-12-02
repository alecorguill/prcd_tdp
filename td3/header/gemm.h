#ifndef GEMM_H
#define GEMM_H
void gemm(const int m, const int n, const int K, const double *A,const int lda, const double *B, const int ldb, double *C, const int ldc);

#endif
