#include <stdio.h>
#include <stdlib.h>
#include <util.h>
#include <string.h>
#include <unistd.h>
#include <cblas.h>
#include <math.h>

double* parse_matrix(char* file, double *M){
  FILE* f = fopen(file,"r");
  if (f == NULL){
    perror("fopen: cannot open the file\n");
    return EXIT_FAILURE;
  }
  char line[MAX_LINE];
  int N = atoi(fgets(line,MAX_LINE,fd));
  int i = 0;
  char *token; char delim[] = " ";
  while(fgets(line,MAX_LINE,fd)){
    token = strtok(line,delim);
    for (int j = 0; j < N; j++){
       token = strtok(NULL,delim);
       *(M+j+i*N) = atof(token);
    }
    i++;
  }
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
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++){
      resi[i+j*n] = cpar[i+j*n] - cseq[i+j*n];
    }

  ///
  double m[n*n];
  cblas_dgemm('t','n',n,n,n,1,resi,n,resi,n,1,m,n);
  double norm = 0.0;

  for (int i = 0; i < n*n; i++)
    norm += m[i];
  norm = sqrt(norm);
  return (norm/(n*epsilon) < 10.0)
} 

// Don't forget to initialize srand(NULL) 
void init_matrix_randomly(double* a, int m, int n, int lda){
  for (int i = 0; i<m; i++)
    for (int j = 0; j<m; j++)
      a[j*lda+i] = (double) rand();
}
