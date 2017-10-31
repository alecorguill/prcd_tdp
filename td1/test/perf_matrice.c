#include <stdio.h>
#include <stdlib.h>
#include "perf.h"
#include "util.h"
#include "assert.h"
#include "cblas.h"
#include "blas.h"


int main(){
  perf_t start,stop;
  
  FILE * fp;
  fp = fopen ("test/flop_matrice.csv", "w+");
  fprintf(fp,"size,flop, ijk, kij, jik\n");
  int m, MAX; m = 100; MAX = 1000;
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
    cblas_dgemm_scalaire(m,b,m,b,m,c,m);
    perf(&stop);
    perf_diff(&start, &stop);
    performance_ijk = perf_mflops(&stop, flop); 
    
    //
    perf(&start);
    cblas_dgemm_scalaire_kij(m,b,m,b,m,c,m);
    perf(&stop);
    perf_diff(&start, &stop);
    performance_kij = perf_mflops(&stop, flop); 
 
    //
    perf(&start);
    cblas_dgemm_scalaire_jik(m,b,m,b,m,c,m);
    perf(&stop);
    perf_diff(&start, &stop);
    performance_jik = perf_mflops(&stop, flop); 
    
    printf("Mflop/s %d: \n ijk : %lf - kij : %lf - jik : %lf\n", m,performance_ijk,
    	   performance_kij, performance_jik);
    fprintf(fp, "%d, %.4f, %.4f, %.4f\n", m,performance_ijk,performance_kij,
	    performance_jik);
    
    m += 10;
 }
  fclose(fp);
  return 0;
}
