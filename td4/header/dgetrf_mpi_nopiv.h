#ifndef DGETRF_MPI_NOPIV_H
#define DGETRF_MPI_NOPIV_H

void dgetrf_mpi_nopiv(const enum CBLAS_ORDER order, const int M, const int N, double *A,
		  const int lda);
#endif
