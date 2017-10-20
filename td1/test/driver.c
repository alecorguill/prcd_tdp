#include "driver.h"

/* Tests des fonctions du fichier util.c*/

/* void affiche(int m, int n, double* a, int lda, int fd); */

void test_affiche(){
  // test unitaire
  double a[] = {1,2,3,4};
  int m, n; m = 2; n = 2;
  int lda; lda = m;
  int fd = 1; // stdout
  
  print("test_affiche 1 ...");
  assert affiche(m, n, a, lda, fd);
  print("ok\n");
  // test fonctionnelle 
  /* double *a = alloue_matrice(); */
  /* print("test_affiche 2 ..."); */
  /* affiche(m, n, a, lda, fd); */
  /* print("ok\n"); */

}

/* void init_matrice(double* a, int m, int n, int lda, double value)*/
void test_init_matrice(){
  int m,n; m = 2; n = m;
  int lda  = m;
  int value = 0;
  double *a = alloue_matrice(m, n);
  init_matrice(a,m,n,value);
  print("test_init_matrice ...");
  int cmp = 0;
  int current_double = 0;
  while(cmp < n*m){
    cmp++;
    if(cmp % n == 0){
      current_double += lda;
    }
    else
      current_double += 1;
    assert a[current_double] != value

  }
  print("ok\n");
}


