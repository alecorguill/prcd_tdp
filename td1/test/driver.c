#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cblas.h"
#include "blas.h"
#include "driver.h"
#include "util.h"
#include "assert.h"



/* Tests des fonctions du fichier util.c*/

/* void affiche(int m, int n, double* a, int lda, int fd); */

void test_affiche(){
  // test unitaire
  double a[] = {1,2,3,4};
  int m, n; m = 2; n = 2;
  int lda; lda = m;
  int fd =  1; // stdout
  
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
  assert(cblas_ddot(3, b, 1, b+3, 3) == 48.0);
  assert(cblas_ddot(2, b+4, 1, b+8, 3) == 117.0);
  printf("ok\n");  
}


/* void cblas_dgemm_scalaire(const int m, const double *A,const int lda, 
			  const double *B, const int ldb,
			  double *C, const int ldc){
*/
void test_dgemm_scalaire(){
  printf("test_dgemm ...\n");
  double b[]   = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0};
  double c[]   = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  printf("\ntest_ijk ...\n");
  fflush(stdout);
 
  cblas_dgemm_scalaire(3,b,3,b,3,c,3);
  affiche(3,3,c,3,1);
  printf("test_kij ...\n");
  fflush(stdout);
  
  cblas_dgemm_scalaire_kij(3,b,3,b,3,c,3);
  affiche(3,3,c,3,1);
  
  printf("test_jik ...\n");
  fflush(stdout);
  cblas_dgemm_scalaire_jik(3,b,3,b,3,c,3);
  affiche(3,3,c,3,1);
   
  printf("ok\n");  
}

void test_daxpy(){
  printf("test_daxpy ...\n");
  int N = 12; 
  int alpha = 1;
  double b[]   = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
  cblas_daxpy(N,alpha,b,0,b,0);
  affiche(N,1,b,N,1);
  printf("ok\n");  
}

void test_dgemv(){
  // matrice et vecteurs remplis de 1, y -> vecteur rempli de 11
  printf("test_dgemv ...\n");
  int n = 10;
  double *A = alloue_matrice(n, n);
  double *x = alloue_matrice(n, 1);
  double *y = alloue_matrice(n, 1);
  init_matrice(A,n,n,n,1);
  init_matrice(y,n,1,n,1);
  init_matrice(x,n,1,n,1);
  cblas_dgemv(0,0,n,n,1,A,n,x,0,1,y,0);
  affiche(n,1,y,n,1);
  free(A); free(x); free(y);
  printf("ok\n");
}


void test_dger(){
  // matrice et vecteurs remplis de 1, y -> vecteur rempli de 11
  printf("test_dger ...\n");
  int n = 10;
  double *A = alloue_matrice(n, n);
  double *x = alloue_matrice(n, 1);
  double *y = alloue_matrice(n, 1);
  init_matrice(A,n,n,n,0);
  init_matrice(y,1,n,n,1);
  init_matrice(x,n,1,n,1);
  cblas_dger(0,n,n,1,x,0,y,0,A,n);
  affiche(n,n,A,n,1);
  free(A); free(x); free(y);
  printf("ok\n");
}


/* void test_init_aleatoire(){ */
/*   printf("test_aleatoire ...\n"); */
/*   int n = 10; */
/*   double *A = alloue_matrice(n, n); */
/*   init_matrice_aleatoire(A,n,n,n); */
/*   affiche(n,n,A,n,1); */
/*   free(A); */
/*   printf("ok\n"); */
  
/* } */


int main(){
  setbuf(stdout, NULL); // disable buffering
  srand(time(NULL)); // random generation
  test_affiche();
  test_init_matrice();
  test_ddot();
  test_dgemm_scalaire();
  test_daxpy();
  test_dgemv();
  test_dger();
  return 0;
}
