#ifndef DDOT_H
#define DDOT_H

/*
  Produit scalaire X.Y
  m : dimension
  dx : vecteur X
  incx : decalage pour le premier element du produit sur X
  dy : vecteur y
  incy : decalage du premier element du produit sur Y
 */
double ddot(const int m, const double *dx, const int incx, const double *dy, const int incy);

#endif