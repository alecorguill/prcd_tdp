#include <stdio.h>
#include <stdlib.h>
#include "perf.h"
#include "util.h"
#include "assert.h"
#include "mkl.h"


int main(){
  perf_t start,stop;
  
  FILE * fp;
  fp = fopen ("test/flop_matrice.csv", "w+");
  fprintf(fp,"size, ijk, kij, jik\n");
  int m, MAX; m = 100; MAX =1000;
  double *b = alloue_matrice(MAX, MAX);
  double *c = alloue_matrice(MAX, MAX);
  init_matrice(b, MAX, MAX, MAX, 0);
  init_matrice(c, MAX, MAX, MAX, 0);
  int flop;
  double performance_ijk, performance_kij,performance_jik;  
  while (m < MAX){
    flop = 2 * m*m*m;
    //
    perf(&start);
    cblas_dgemm(0,0,0,m,m,m,1,b,m,b,m,1,c,m);
    perf(&stop);
    perf_diff(&start, &stop);
    performance_ijk = perf_mflops(&stop, flop); 

    printf("Mflop/s %d: \n ijk : %lf\n", m, performance_ijk);
    fprintf(fp, "%d, %.4f\n", m,performance_ijk);
    
    m += 10;
 }
  fclose(fp);
  return 0;
}
