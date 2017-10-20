#include "ddot.h"

/*
  Produit scalaire X.Y
  m : dimension
  dx : vecteur X
  incx : decalage pour le premier element du produit sur X
  dy : vecteur y
  incy : decalage du premier element du produit sur Y
 */
double ddot(const int m, const double *dx, const int incx, 
	    const double *dy, const int incy){
  double resultat = 0.0;
  int i = 0;
  while (i < m){
    resultat += *(dx+incx*i) * *(dy+incy*i);
    i++;
  }
  return resultat;

}
