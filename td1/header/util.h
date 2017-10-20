#ifndef UTIL_H
#define UTIL_H

void affiche(int m, int n, double* a, int lda, int fd); 
double * alloue_matrice(int m, int n)
double * alloue_vecteur(int m);
void init_matrice(double* a, int m, int n, int lda, double value);
void init_vecteur(double* a, int m, double value);
void free_matrice(int m, int n);
void free_vecteur(int m);
#endif