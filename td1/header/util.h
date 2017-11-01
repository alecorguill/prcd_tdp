#ifndef UTIL_H
#define UTIL_H

void affiche(int m, int n, double* a, int lda, int fd); 
double * alloue_matrice(int m, int n);
double * alloue_vecteur(int m);
void init_matrice(double* a, int m, int n, int lda, double value);
void somme_matrice(const int m, const int n,const double *A,const int lda, 
		   const double *B, const int ldb, double *C, const int ldc);
int equal_matrice(const int m, const int n, const double *A,const int lda, 
		   const double *B, const int ldb);

void init_matrice_aleatoire(double* a, int m, int n, int lda);

#endif
