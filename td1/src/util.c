#include <stdio.h>
#include <stdlib.h>

#include "util.h"


/*
  Alloue une matrice A de dimension m*n
  Parametres :
    m : nombre de lignes
    n : nombre de colonnes

  Retour : Un pointeur sur A.
 */

double * alloue_matrice(int m, int n){
  return (double * ) malloc(sizeof(double) *n*m);
}

/*
  Alloue un vecteur V de dimension m.
  Parametres :
    m : dimension du vecteur

  Retour : Un pointeur sur un V 
 */
double * alloue_vecteur(int m){
  return (double * ) malloc(sizeof(double)*m);
}

/*
  Initialise une matrice avec la valeur value.
  Parametres :
    a : adresse de la matrice
    m : nombre de ligne
    n : nombre de colonnes
    lda : leading dimension
    value : valeur d'initialisation

  Retour : void
 */
void init_matrice(double* a, int m, int n, int lda, double value){
  int i = 0;
  int j = 0;
  while(i < m){
    while(j < n){
      a[j*lda+i] = value;
      ++j;
    }
    j=0;
    ++i;
  }
}

/*
  Affiche une matrice.
  Parametres :
    a : adresse de la matrice
    m : nombre de ligne
    n : nombre de colonnes
    lda : leading dimension
    fd : file descritor pour rediriger l'affichage

  Retour : void
 */
void affiche(int m, int n, double* a, int lda, int fd){
  int i = 0;
  int j = 0;
  while(i < m){
    while(j < n){
      dprintf(fd, "%f ", a[j*lda+i]);
      ++j;
    }
    j=0;
    ++i;
    dprintf(fd, "\n");
  }
}
