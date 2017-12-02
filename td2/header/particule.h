#ifndef PARTICULE_H
#define PARTICULE_H

#define G 6.67408E-11

#include <stdio.h>
 
/* Position en 2d */
struct position{
  double x;
  double y;
};

typedef struct position vecteur;
  
/* Represente une particule */
struct particule{
  int m; //masse
  vecteur p; //position
  vecteur v; //vitesse
  vecteur a; //acceleration
  vecteur f_ext; //force exercées sur la particul à l'instant t
  double proche_d; //la distance correspondant à la particule la plus proche
};

typedef struct particule particule;


double norme(vecteur *v);
void somme(vecteur *v1, vecteur *v2, vecteur *res);
void diff(vecteur *v1, vecteur *v2, vecteur *res);
void scal_prod(vecteur *v1, double alpha, vecteur *res);
double distance(particule *p1, particule *p2);
void force_grav(particule *p1, particule *p2, vecteur *force);
void update_acceleration(particule *p);
void update_vitesse(particule *p, double dt);
void update_position(particule *p, double dt);
void update_particules(particule *univers,int nb_particule, double dt);
void log_particules(particule *univers,int nb_particule, int output);
void print_particule(particule *p);
void log_particules_par(particule*,int,int,double,int,int);

#endif
