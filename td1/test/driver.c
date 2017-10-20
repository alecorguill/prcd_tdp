#include <stdio.h>
#include <stdlib.h>

#include "driver.h"
#include "util.h"
#include "ddot.h"
#include "assert.h"

/* Tests des fonctions du fichier util.c*/

/* void affiche(int m, int n, double* a, int lda, int fd); */

void test_affiche(){
  // test unitaire
  double a[] = {1,2,3,4};
  int m, n; m = 2; n = 2;
  int lda; lda = m;
  int fd = 1; // stdout
  
  printf("test_affiche 1 ...");
  affiche(m, n, a, lda, fd);
  printf("ok\n");
 
  m = 1; n = 4, lda=m;
  printf("test_affiche 2 ...");
  affiche(m, n, a, lda, fd);
  printf("ok\n");
 
  double b[]   = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
  m = 2; n = 3;
  int ldb = 3;
  fd = 1; // stdout
  
  printf("test_affiche 3 ...");
  affiche(m, n, b, ldb, fd);
  printf("ok\n");
}

/* void init_matrice(double* a, int m, int n, int lda, double value)*/
void test_init_matrice(){
  int m,n; m = 2; n = m;
  int lda  = m;
  int value = 0;
  double *a = alloue_matrice(m, n);
  init_matrice(a,m,n,lda,value);
  printf("test_init_matrice ...");
  int i = 0;
  int j = 0;
  while(i < m){
    while(j < n){
      assert(a[j*lda+i] == value);
      ++j;
    }
    j=0;
    ++i;
  }
  free(a);
  printf("ok\n");
}

/*double ddot(const int m, const double *dx, const int incx, const double *dy, const int incy) */
void test_ddot(){
  printf("test_ddot ...");
  double b[]   = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
  int m, n; m = 3; n = 4;
  int lda; lda = m;
  assert(ddot(3, b, 1, b+3, 3) == 48.0);
  assert(ddot(2, b+4, 1, b+8, 3) == 117.0);
  printf("ok\n");  
}

int main(){
  test_affiche();
  test_init_matrice();
  test_ddot();
  return 0;
}
