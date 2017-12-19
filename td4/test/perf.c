#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cblas.h"
#include "util.h"
#include "assert.h"
#include "dgetf2_nopiv.h"

void test_norme2(){
  int m = 4;
  int n = 4;
  double A[12] = {1,2,3.1,4,5,6,7,8,9,10,11,12};
  double C[12];
  print(m,n,A,m,1);
  printf("\n");
  
  dgetf2_nopiv(CblasColMajor,m,n,A,m);
  print(m,n,A,m,1);
  printf("\n");
  for(int i=0;i<m*n;++i)
    C[i] = 0.0;
    
  cblas_dgemm_lu(m,n,A,m,C,m);
  print(m,n,C,m,1);
  printf("\n");
  
  /* random_matrix(m,n,0,10,A,m); */
  /* diff_matrix(m,n,A,m,A,m,A,m); */

}

void test_dgetf2_nopiv(){ 
  printf("test_dgetrf2_nopiv ...\n");
  int n = 4;
  int m = 4;
  double A[m*n];
  random_matrix(m,n,0,10,A,m);
  print(m,n,A,m,1);
  printf("\n");
  dgetf2_nopiv(CblasColMajor,m,n,A,m);
  print(m,n,A,m,1);
  double C[m*n];
  for(int i=0;i<m*n;++i)
    C[i] = 0.0;
  printf("\n");
  cblas_dgemm_lu(m,n,A,m,C,m);
  print(m,n,C,m,1);
  printf("ok\n");
}


int main(){
  test_norme2();
  //test_dgetf2_nopiv();
  return 0;
}
