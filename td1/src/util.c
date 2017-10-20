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
  Initialise un vecteur avec la valeur value.
  Parametres :
    a : adresse du vecteur
    m : dimension du vecteur
    value : valeur d'initialisation

  Retour : void
 */
void init_vecteur(double* a, int m, double value){
  for(int i=0; i<m; ++i)
      a[i] = value;
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
  int cmp = 0;
  int current_double = 0;
  while(cmp < n*m){
    a[current_double] = value;
    cmp++;
    if(cmp % n == 0)
      current_double += lda;
    else
      current_double += 1;
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
  int cmp = 0;
  int current_double = 0;
  while(cmp < n*m){
    dprintf(fd, "%f ", a[current_double])
    cmp++;
    if(cmp % n == 0){
      current_double += lda;
      dprintf(fd, "\n")
    }
    else
      current_double += 1;
  }
}