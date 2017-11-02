#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"
#include "util.h"
#include "perf.h"
#include "assert.h"
#include "mkl.h"

int main() {
  perf_t start;
  perf_t stop;
  double mflops;
  int m,flop; m=100;
  int MAX;MAX=1000000;
  double *b = malloc(sizeof(double) * MAX*2);
  //memset(b, 1.5, MAX*2);
  /* init_matrice(b,MAX,1,MAX,0); */
  init_matrice_aleatoire(b,MAX,1,MAX);
  FILE * fp;
  fp = fopen ("test/flop_vecteur_a.csv", "w+");
  /* fp = fopen ("test/flop_vecteur.csv", "w+"); */
  fprintf(fp,"size,flop\n");
  while(m <= MAX){
    flop = 2*m-1;
    perf(&start);
    ddot(m, b, 1, b+m, 1);
    perf(&stop);
    perf_diff(&start,&stop);
    perf_printh(&stop);
    perf_printmicro(&stop);
    mflops = perf_mflops(&stop, flop);
    fprintf(fp, "%d, %.4f\n", m, mflops);
    m += m/4;
  }
  fclose(fp);
  free(b);
  return 0;
}
