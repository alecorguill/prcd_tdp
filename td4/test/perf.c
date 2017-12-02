#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cblas.h"
#include "util.h"
#include "assert.h"
#include "dgetf2_nopiv.h"

void test_dgetf2_nopiv(){ 
  printf("test_dgetrf2_nopiv ...\n");
  int n = 3;
  int m = 4;
  double A[] = {2,5,2,0,-2,8,1,1,3,-1,1,3};
  print(m,n,A,m,1);
  printf("\n");
  dgetf2_nopiv(CblasColMajor,m,n,A,m);
  print(m,n,A,m,1);
  printf("ok\n");
}


int main(){
  test_dgetf2_nopiv();
  return 0;
}
