#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#include "cblas.h"
#include "util.h"
#include "assert.h"
#include "dgetf2_nopiv.h"
#include "dgetrf_nopiv.h"

#define MAX_LENGTH 20

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
  int m = 41;
  int n = 57;
  double A[m*n],C[m*n],tmp[m*n],tmp2[m*n];
  for(int i=0; i<n*m; ++i){
    C[i] = 0.0;
  }
  random_matrix(m,n,0,10,A,m);   
  memcpy(tmp,A,n*m*sizeof(double));
  dgetrf_nopiv(CblasColMajor,m,n,A,m);
  for(int i=0;i<m*n;++i)
    C[i] = 0.0;    
  cblas_dgemm_lu(m,n,A,m,C,m);
  /* printf("C\n"); */
  /* print(m,n,C,m,1); */
  /* printf("tmp\n"); */
  /* print(m,n,tmp,m,1); */
  double abs_err = absolute_error(m,n,tmp,m,C,m);
  double rel_err = relative_error(m,n,tmp,m,C,m);
  assert(abs_err < eps);
  assert(rel_err < eps);
  printf("ok\n");
}

void create_csv_lu(int nmax, lu_function lu, char* function_name){
  printf("Creating csv..");
  int n=2,min=0,max=10;    
  double abs_err,rel_err;
  double * tmp,*A,*res,duration;
  struct timeval t1, t2;
  if(strlen(function_name) > MAX_LENGTH){
    puts("Function name too long");
    exit(EXIT_FAILURE);
  }

  char abs[]  = "_abs_err.csv";
  char rel[]  = "_rel_err.csv";
  char time[] = "_time.csv";
  int char_size = MAX_LENGTH + strlen(rel);
  char abs_csv[char_size];
  char rel_csv[char_size];
  char time_csv[char_size];
  strcpy(abs_csv,function_name);
  strcpy(rel_csv,function_name);
  strcpy(time_csv,function_name);
  strcat(time_csv,time);
  strcat(rel_csv,rel);
  strcat(abs_csv,abs);
  int abs_file = open(abs_csv, O_CREAT | O_WRONLY | O_TRUNC,0444);
  int rel_file = open(rel_csv, O_CREAT | O_WRONLY | O_TRUNC,0444);
  int time_file = open(time_csv, O_CREAT | O_WRONLY | O_TRUNC,0444);
  if ((abs_file < 0) || (rel_file < 0) || (time_file < 0)){
    perror("open : csv files\n");
    exit(EXIT_FAILURE);
  }
  dprintf(abs_file,"n,absolute_error\n");
  dprintf(rel_file,"n,relative_error\n");
  dprintf(time_file,"n,time\n");

  tmp  = malloc(n*n*sizeof(double));
  A    = malloc(n*n*sizeof(double));
  res  = malloc(n*n*sizeof(double));
  while(n<nmax){
    for(int i=0; i<n*n; ++i){
      tmp[i] = 0.0;
      A[i]   = 0.0;
      res[i] = 0.0;
    }
    random_matrix(n,n,min,max,A,n);   
    memcpy(tmp,A,n*n*sizeof(double));
    gettimeofday(&t1, NULL);
    lu(CblasColMajor,n,n,A,n);
    gettimeofday(&t2, NULL);
    cblas_dgemm_lu(n,n,A,n,res,n);
    abs_err = absolute_error(n,n,tmp,n,res,n);
    rel_err = relative_error(n,n,tmp,n,res,n);
    duration = (t2.tv_sec - t1.tv_sec) * 10E6 + (t2.tv_usec - t1.tv_usec);
    dprintf(abs_file,"%d,%.14f\n",n,abs_err);
    dprintf(rel_file,"%d,%.14f\n",n,rel_err);
    dprintf(time_file,"%d,%f\n",n,duration);
    n = (int) n*1.5;
    free(tmp);free(A);free(res);
    tmp  = malloc(n*n*sizeof(double));
    A    = malloc(n*n*sizeof(double));
    res  = malloc(n*n*sizeof(double)); 
  }    
  free(tmp);free(A);free(res);
  printf("ok\n");

}


void create_csv_lu_bloc(int n, int size_b_max, lu_function lu_block, 
			char* function_name){
  printf("Creating csv..");
  int min=0,max=10;
  double abs_err,rel_err;
  double * tmp,*A,*res,duration;
  struct timeval t1, t2;
  if(strlen(function_name) > MAX_LENGTH){
    puts("Function name too long");
    exit(EXIT_FAILURE);
  }

  char abs[]  = "_abs_bloc_err.csv";
  char rel[]  = "_rel_bloc_err.csv";
  char time[] = "_time_bloc.csv";
  int char_size = MAX_LENGTH + strlen(rel);
  char abs_csv[char_size];
  char rel_csv[char_size];
  char time_csv[char_size];
  strcpy(abs_csv,function_name);
  strcpy(rel_csv,function_name);
  strcpy(time_csv,function_name);
  strcat(time_csv,time);
  strcat(rel_csv,rel);
  strcat(abs_csv,abs);
  int abs_file = open(abs_csv, O_CREAT | O_WRONLY | O_TRUNC,0444);
  int rel_file = open(rel_csv, O_CREAT | O_WRONLY | O_TRUNC,0444);
  int time_file = open(time_csv, O_CREAT | O_WRONLY | O_TRUNC,0444);
  if ((abs_file < 0) || (rel_file < 0) || (time_file < 0)){
    perror("open : csv files\n");
    exit(EXIT_FAILURE);
  }
  dprintf(abs_file,"size_block,absolute_error\n");
  dprintf(rel_file,"size_block,relative_error\n");
  dprintf(time_file,"size_block,time\n");

  tmp  = malloc(n*n*sizeof(double));
  A    = malloc(n*n*sizeof(double));
  res  = malloc(n*n*sizeof(double));
  int size_b=2;
  char str[16];
  while(size_b<size_b_max){
    for(int i=0; i<n*n; ++i){
      tmp[i] = 0.0;
      A[i]   = 0.0;
      res[i] = 0.0;
    }
    sprintf(str, "%d", size_b);
    setenv("BLOCK_SIZE",str,1);
    random_matrix(n,n,min,max,A,n);   
    memcpy(tmp,A,n*n*sizeof(double));
    gettimeofday(&t1, NULL);
    lu_block(CblasColMajor,n,n,A,n);
    gettimeofday(&t2, NULL);
    cblas_dgemm_lu(n,n,A,n,res,n);
    abs_err = absolute_error(n,n,tmp,n,res,n);
    rel_err = relative_error(n,n,tmp,n,res,n);
    duration = (t2.tv_sec - t1.tv_sec) * 10E6 + (t2.tv_usec - t1.tv_usec);
    dprintf(abs_file,"%d,%.14f\n",size_b,abs_err);
    dprintf(rel_file,"%d,%.14f\n",size_b,rel_err);
    dprintf(time_file,"%d,%f\n",size_b,duration);
    size_b = ((int) size_b*1.3) + 1;
  }    
  free(tmp);free(A);free(res);
  printf("ok\n");

}



int main(int argc, char** argv){
  if(argc < 4){
    printf("USAGE : %s bool_csv eps n_max\n",argv[0]);
    printf("Exemple : %s 1 10E-8 100\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  int create_csv = atoi(argv[1]);
  double eps = strtod(argv[2],NULL);
  int nmax = atoi(argv[3]);
  printf("Tests lancés avec un erreur max de : %.14f\n", eps);
  if(create_csv){
    create_csv_lu(nmax,dgetf2_nopiv,"normal");
    create_csv_lu(nmax,dgetrf_nopiv,"bloc");
    create_csv_lu_bloc(nmax,(int) nmax*0.5,dgetrf_nopiv,"bloc");
      
  }
  test_dgetf2_nopiv(eps);
  test_dgetrf_nopiv(eps);
  return 0;
}
