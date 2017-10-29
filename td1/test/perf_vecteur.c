#include <stdio.h>
#include <stdlib.h>
#include "driver.h"
#include "util.h"
#include "perf.h"
#include "ddot.h"
#include "assert.h"
#include "dgemm.h"

int main() {
  perf_t start;
  perf_t stop;
  double mflops;
  int m,flop; m=100;
  int MAX;MAX=1000000;
  double *b = malloc(sizeof(double) * MAX*2);
  FILE * fp;
  fp = fopen ("test/flop_vecteur.csv", "w+");
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
