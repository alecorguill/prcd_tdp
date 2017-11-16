#include <util.h>
#include <particule.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_LIGNE 81

/* Compare deux doubles pour palier au problème sur le flottants */
int equal_double(double a, double b){
  return fabs(a-b) < EPS;
}

/* Calcul le nouveau pas de temps */
double nouveau_dt(particule* univers, int m){
  int n = 0;
  double d_min = 0.0;
  double d;
  while (n < m){
    d = solution_equ(univers[n]);
    if (d_min == 0.0 || d_min > d)
	d_min = d; 
    n++;
  }
  return d_min;
}

/* parse le fichier de particules et rempli la structure */
void parse_particules(char *filename, particule *ps){
  FILE *f = NULL;
  char ligne[TAILLE_LIGNE];
  f=fopen(filename, "r");
  if(f == NULL)
    perror("FOPEN");
  fgets(ligne, TAILLE_LIGNE, f);
  while ( fgets(ligne, TAILLE_LIGNE, f) != NULL ){
    char *token;
    char delim[] = " ";
    token = strtok(ligne, delim);
    ps->m = atoi(token);
    token = strtok(NULL, delim);
    ps->p.x = atof(token);
    token = strtok(NULL, delim);
    ps->p.y = atof(token);
    token = strtok(NULL, delim);
    ps->v.x = atof(token);
    token = strtok(NULL, delim);
    ps->v.y = atof(token);
    ps->a.x = 0.0;
    ps->a.y = 0.0;
    ps->proche_d = 0.0;
    ++ps;
  }
  fclose(f);
}


/* solution à l équation de dt pour garantir la condition de l'énoncé */
double solution_equ(particule p){
  double a = norme(&p.a) / 2;
  double b = norme(&p.v);
  double c = - 0.1 * p.proche_d;
  double delta = b*b - 4*a*c;
  double x;
  if(equal_double(a,0.0))
    return -c/b;
  x = (-b + sqrt(delta)) / (2 * a);
  return x;
}

//////////////////////////////////////////////////
#ifdef MPIFLAG
#include <mpi.h>
/* comme parse_particules mais en parallele */
void parse_particule_par(char* filename, int rank, particule* univers){
  
  int nb; char ligne[MAX];  
  FILE* fd = fopen(filename, "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    exit(EXIT_FAILURE);
  }
  fgets(ligne, MAX, fd);
  nb = atoi(ligne);
 
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Lit le fichier initial
  // Récupere les valeurs liées à son rang
  int i = 0;
  int j = 0;
  while (i < nb){
    fgets(ligne,MAX,fd);
    if (i % size == rank){
      sscanf(ligne, "%d %lf %lf %lf %lf", &((univers+j)->m),&((univers+j)->p.x),
      	     &((univers+j)->p.y), &((univers+j)->v.x),&((univers+j)->v.y));
      (univers+j)->a.x = 0.0;
      (univers+j)->a.y = 0.0;
      (univers+j)->proche_d = 0.0;
    
      j++;
    }
    i++;
  }
  fclose(fd);
}


#endif 

