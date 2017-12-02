#include <stdio.h>
#include <stdlib.h>
#include <util.h>
#include <string.h>
#include <unistd.h>

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





