#include <particule.h>

/* Calcul la norme d'un vecteur */
double norme(vecteur *v){
  vecteur diff = {v->p2.x-v->p1.x, v->p2.y-v->p1.y}
  return diff.x*diff.x + diff.y+diff.y
}

void somme(vecteur *v1, vecteur *v2, vecteur *res){
  return {v1->x+v2->x, v1->y+v2->y}; 
}