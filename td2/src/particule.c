#include <particule.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define LINESIZE 80 /* Taille max de nos lignes */

/* Calcul la norme d'un vecteur */
double norme(vecteur *v){
  return sqrt(v->x*v->x+v->y*v->y);
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
  res->x = alpha*v->x;
  res->y = alpha*v->y;
  return;
}

/* retourne la distance entre 2 particules */
double distance(particule *p1, particule *p2){
  vecteur v;
  diff(&p2->p, &p1->p, &v);
  return norme(&v);
}

/* calcul la force de p2 sur p1 */
void force_grav(particule *p1, particule *p2, vecteur *force){
  double dst = distance(p1,p2);
  double coef = G*(p1->m+p2->m)/(dst*dst);
  force->x = coef*(1/dst)*(p1->p.x-p2->p.x);
  force->y = coef*(1/dst)*(p1->p.y-p2->p.y);
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
  vecteur v;
  scal_prod(&p->a,dt, &v);
  somme(&p->v, &v, &p->v);
  return;
}

/*
 * Met à jour la position de la particule en fonction de la vistesse
 */
void update_position(particule *p, double dt){
  vecteur v;
  vecteur v2;
  scal_prod(&p->a,dt*dt/2, &v);
  scal_prod(&p->v,dt, &v2);
  somme(&v, &v2, &v);
  somme(&v, &p->p, &p->p);
  return;
}

/* Encapsule les updates (position, vitesse, acceleration) */
void update_particules(particule *univers,int nb_particule, double dt){
  int k=0;
  while (k < nb_particule){	
    update_acceleration(&univers[k]);
    update_position(&univers[k], dt);	    
    update_vitesse(&univers[k], dt);
    k++;
  }
}

/* Log les infos d'une particule dans un fichier */
void log_particules(particule *univers,int nb_particule, int output){
  int k=0;
  while (k < nb_particule){	
    dprintf(output, "%lf,%lf\n", univers[k].p.x, univers[k].p.y);
    k++;
  }
}


/* affiche une particule */
void print_particule(particule *p){
    printf("masse : %d\n", p->m);
    printf("position : %lf %lf ", p->p.x, p->p.y);
    printf("vitesse : %lf %lf\n", p->v.x, p->v.y);   
    printf("acceleration : %lf %lf\n", p->a.x, p->a.y);
    printf("force exterieur : %lf %lf\n", p->f_ext.x, p->f_ext.y);
    printf("proche_d : %lf\n", p->proche_d);
    fflush(stdout);
}
//////////////////////////////////////////////////
#ifdef MPIFLAG
#include <mpi.h>
/* Même chose que log_particules mais en parallele */
void log_particules_par(particule *univers,int alpha, int output, 
			double t, int root, int iteration){
  int rank, size;
  off_t offset;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  offset = LINESIZE * (iteration * (alpha+1+(size-1)*alpha) + (rank != root) * (alpha+1+(rank-1)*alpha));
  lseek(output,offset,SEEK_SET);
  if (rank == root){
      dprintf(output, "%lf\n", t);
  }
  log_particules(univers,alpha,output);
}
#endif 
