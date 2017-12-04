#define _POSIX_C_SOURCE  200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "util.h"
#include "gemm.h"

double* parse_matrix(char* file, double *M){
  FILE* f = fopen(file,"r");
  if (f == NULL){
    perror("fopen: cannot open the file\n");
    exit(EXIT_FAILURE);
  }
  char line[MAX_LINE];
  int N = atoi(fgets(line,MAX_LINE,f));
  int i = 0;
  char *token; char delim[] = " ";
  while(fgets(line,MAX_LINE,f)){
    token = strtok(line,delim);
    for (int j = 0; j < N; j++){
      *(M+j*N+i) = atof(token);
      token = strtok(NULL,delim);
      
    }
    i++;
  }
  fclose(f);   
  return M;
}

void print_matrix(double* A, int N, int fd){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++)
      dprintf(fd, "%f ", A[j*N+i]);
    dprintf(fd, "\n");
  }
}

int equal(double* cseq, double* cpar, int n, double epsilon){

  double resi[n*n];
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      resi[i+j*n] = cpar[i+j*n] - cseq[i+j*n];
    }
  }
  ///
  double m[n*n];
  gemm(n,n,n,resi,n,resi,n,m,n);
  double norm = 0.0;

  for (int i = 0; i < n*n; i++)
    norm += m[i];
  norm = sqrt(norm);
  return (norm/(n*epsilon) < 10.0);
} 

// Don't forget to initialize srand(NULL) 
void init_matrix_randomly(double* a, int m, int n, int lda){
  for (int i = 0; i<m; i++)
    for (int j = 0; j<n; j++)
      a[j*lda+i] = (double) rand();
}

