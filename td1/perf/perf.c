#include "perf.h"
#include <stdlib.h>
#include <stdio.h>
#include "ddot.h"

void 
perf(perf_t * p) {
  gettimeofday(p, NULL);  
}

void 
perf_diff(const perf_t * begin, perf_t * end) {
  end->tv_sec = end->tv_sec - begin->tv_sec;
  end->tv_usec = end->tv_usec - begin->tv_usec;
  if (end->tv_usec < 0) {
    (end->tv_sec)--;
    end->tv_usec += 1000000;
  }
}

void
perf_printh(const perf_t * p) {
  long m = p->tv_sec / 60;
  long s = p->tv_sec - m*60;
  long ms = p->tv_usec / 1000;
  long micros = p->tv_usec - ms*1000;

  //  printf("%ld sec %ld usec\n", p->tv_sec, p->tv_usec);
  printf("%ld:%ld:%ld:%ld\n",m,s,ms,micros);
}

void
perf_printmicro(const perf_t * p) {
  printf("%ld\n",p->tv_usec + ( p->tv_sec * 1000000) );
}


double
perf_mflops(const perf_t * p, const long nb_op) {
  return (double)nb_op / (p->tv_sec * 1000000 + p->tv_usec);
}


int 
main() {
  perf_t start;
  perf_t stop;
  double b[]   = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
  int m, n; m = 3; n = 4;
  int lda; lda = m;

  perf(&start);
  int flop = 2*m -1;
  
  perf(&stop);
  ddot(3, b, 1, b+3, 3);
  perf_diff(&start,&stop);

  perf_printh(&stop);
  perf_printmicro(&stop);
   
  double mflops = perf_mflops(&stop, flop);

  printf("Mflops : %.4f\n",mflops);

}
