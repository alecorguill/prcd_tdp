#include <particule.h>

/* Calcul la norme d'un vecteur */
double norme(vecteur *v){
  return v->x*v->x+v->y+v->y;
}

/* res=v1+v2 */
void somme(vecteur *v1, vecteur *v2, vecteur *res){
  res->x = v1->x+v2->x;
  res->y = v1->y+v2->y;
  return;
}

/* res = v1-v2 */
void diff(vecteur *v1, vecteur *v2, vecteur *res){
  res->x = v1->x-v2->x;
  res->y = v1->y-v2->y;
  return;
}

/* res = alpha*v */
void scal_prod(vecteur *v,double alpha, vecteur *res){
  res->x *= alpha;
  res->y *= alpha;
  return;
}

/* retourne la distance entre 2 particules */
double distance(particule *p1, particule *p2){
  vecteur v;
  diff(p2->p, p1->p, &v);
  return norme(&v);
}

/* calcul la force de i sur j */
void force_grav(particule *p1, particule *p2, vecteur *force){
  double distance = distance(p1,p2);
  double coef = G*(p1->m+p2->m)/(distance*distance)
  force.x = coef*(1/distance)*(p2.x-p1.x);
  force.y = coef*(1/distance)*(p2.y-p1.y);
  return;
}

/*
 * Met à jour l'accelération en fonction des forces extérieures qui s'appli
 * quent sur la particule
 */
void update_acceleration(particule *p){
  p->a.x = p->f_ext.x/p->m;
  p->a.y = p->f_ext.y/p->m;
}

/*
 * Met à jour la vitesse de la particule en fonction de l'acceleration
 */
void update_vitesse(particule *p, double dt){
  somme(p->v, scal_prod(p->a,dt), p->v);
}

/*
 * Met à jour la position de la particule en fonction de la vistesse
 */
void update_position(particule *p, double dt){
  vecteur v;
  scal_prod(p->a,dt*dt/2, &v);
  somme(&v, scal_prod(p->v,dt));
  somme(p->v, v);
}