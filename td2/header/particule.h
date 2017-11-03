#ifndef PARTICULE_H
#define PARTICULE_H

#define G 5

struct position{
  double x;
  double y;
};

typedef struct position vecteur;
  
struct particule{
  const int m; //masse
  position p; //position
  vecteur v; //vitesse
  vecteur a; //acceleration
  vecteur f_ext; //force exercées sur la particul à l'instant t  
};



double norme(vecteur *v);
void somme(vecteur *v1, vecteur *v2, vecteur *res);
void diff(vecteur *v1, vecteur *v2, vecteur *res);
void scal_prod(vecteur *v1, vecteur *v2, vecteur *res);
void force_grav(particule *p1, particule *p2, vecteur *force);
void update_acceleration(particule *p, double dt);
void update_vitesse(particule *p, double dt);
void update_position(particule *p, double dt);
#endif