#include <util.h>
#include <particule.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define TAILLE_LIGNE 81

int equal_double(double a, double b){
  return fabs(a-b) < EPS;
}
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


double solution_equ(particule p){
  double a = norme(&p.a) / 2;
  double b = norme(&p.v);
  double c = - 0.1 * p.proche_d;
  double delta = b*b - 4*a*c;
  double x;
  if(equal_double(a,0.0))
    return -c/b;
  x = (-b + sqrt(delta)) / (2 * a);
  //printf("%lf %lf %lf DELTA %lf  SOL %lf \n", a,b,c, delta, x);
  return x;
}

//////////////////////////////////////////////////
void parse_particule_par(char * filename, int rank, particule** univers, int* m){
  
  int nb; char ligne[MAX];
  
  FILE* fd = fopen(filename, "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    exit(EXIT_FAILURE);
  }
  fgets(ligne, MAX, fd);
  nb = atoi(ligne);
  *m = nb;

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("size , nb, rank : %d %d %d\n", size,nb,rank);
  int alpha = nb/size;
  particule *univers_tmp = malloc(sizeof(particule)*alpha);
  
  // Lit le fichier initial 
  // Récupere les valeurs liées à son rang
  int i = 0;
  int j = 0;
  while (i < nb){
    fgets(ligne,MAX,fd);
    if (i % size == rank){
      j = i / (rank+1);
      sscanf(ligne, "%d %lf %lf %lf %lf", &((univers_tmp+i)->m),&((univers_tmp+i)->p.x),
	     &((univers_tmp+i)->p.y), &((univers_tmp+i)->v.x),&((univers_tmp+i)->v.y));
  }
    i++;
  }
  fclose(fd);
  *univers = univers_tmp;
  free(univers_tmp);
}

// Arthur dit que ça marche
void init_buffers(int m, particule** com, particule* univers){
  
  for (int i=0; i<m; i++){
    com[0][i] = univers[i];
  }
}

