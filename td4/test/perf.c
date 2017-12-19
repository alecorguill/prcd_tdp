#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cblas.h"
#include "util.h"
#include "assert.h"
#include "dgetf2_nopiv.h"
#include "dgetrf_nopiv.h"

void test_dgetf2_nopiv(double eps){
  printf("Test dgetf2_nopiv..");
  int m = 100;
  int n = 500;
  double A[m*n],C[m*n],tmp[m*n];
  for(int i=0; i<n*m; ++i){
    C[i] = 0.0;
  }
  random_matrix(m,n,0,10,A,m);   
  memcpy(tmp,A,n*m*sizeof(double));
  dgetf2_nopiv(CblasColMajor,m,n,A,m);
  for(int i=0;i<m*n;++i)
    C[i] = 0.0;    
  cblas_dgemm_lu(m,n,A,m,C,m);
  double abs_err = absolute_error(m,n,tmp,m,C,m);
  double rel_err = relative_error(m,n,tmp,m,C,m);
  assert(abs_err < eps);
  assert(rel_err < eps);
  printf("ok\n");
}


void test_dgetrf_nopiv(double eps){
  printf("Test dgetrf_nopiv..");
  int m = 6;
  int n = 5;
  double A[m*n],C[m*n],tmp[m*n];
  for(int i=0; i<n*m; ++i){
    C[i] = 0.0;
  }
  random_matrix(m,n,0,10,A,m);   
  memcpy(tmp,A,n*m*sizeof(double));
  dgetrf_nopiv(CblasColMajor,m,n,A,m);
  for(int i=0;i<m*n;++i)
    C[i] = 0.0;    
  cblas_dgemm_lu(m,n,A,m,C,m);
  double abs_err = absolute_error(m,n,tmp,m,C,m);
  double rel_err = relative_error(m,n,tmp,m,C,m);
  assert(abs_err < eps);
  assert(rel_err < eps);
  printf("ok\n");
}

void create_csv_dgetf2(int nmax){
  printf("Test dgetf2_nopiv..");
  int n=2,min=0,max=10;    
  double abs_err,rel_err;
  double *tmp  = malloc(n*n*sizeof(double));
  double * A   = malloc(n*n*sizeof(double));
  double * res = malloc(n*n*sizeof(double));
  char * abs_csv = "abs_err.csv";
  char * rel_csv = "rel_err.csv";
  int abs_file = open(abs_csv, O_CREAT | O_WRONLY | O_TRUNC,0744);
  int rel_file = open(rel_csv, O_CREAT | O_WRONLY | O_TRUNC,0744);
  if ((abs_file < 0) || (rel_file < 0)){
    perror("open : csv files\n");
    exit(EXIT_FAILURE);
  }
  dprintf(abs_file,"n,absolute error");
  dprintf(rel_file,"n,relative error");
  while(n<nmax){
    memset(tmp,0,n*n);memset(res,0,n*n);memset(A,0,n*n);
    random_matrix(n,n,min,max,A,n);   
    memcpy(tmp,A,n*n*sizeof(double));
    dgetf2_nopiv(CblasColMajor,n,n,A,n);
    cblas_dgemm_lu(n,n,A,n,res,n);
    abs_err = absolute_error(n,n,tmp,n,res,n);
    rel_err = relative_error(n,n,tmp,n,res,n);
    dprintf(abs_file,"%d,%f\n",n,abs_err);
    dprintf(rel_file,"%d,%f\n",n,rel_err);
    n = (int) n*1.5;
  }  

  free(tmp);free(A);free(res);
  printf("ok\n");

}


int main(int argc, char** argv){
  if(argc < 4){
    printf("USAGE : %s bool_csv eps n_max\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  int create_csv = atoi(argv[1]);
  double eps = strtod(argv[2],NULL);
  int nmax = atoi(argv[3]);
  if(create_csv){
    create_csv_dgetf2(nmax);
  }
  printf("Tests lancés avec un erreur max de : %.14f\n", eps);
  test_dgetf2_nopiv(eps);
  test_dgetrf_nopiv(eps);
  return 0;
}
