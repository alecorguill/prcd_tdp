#include <util.h>
#include <particule.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TAILLE_LIGNE 256

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
void parse_particules_bloc(char *filename, bloc * blocs){
  FILE *f = NULL;
  char ligne[TAILLE_LIGNE];
  f=fopen(filename, "r");
  if(f == NULL)
    perror("FOPEN");
  fgets(ligne, TAILLE_LIGNE, f);
  fgets(ligne, TAILLE_LIGNE, f);
  blocs[0].dim = atoi(ligne);
  int ind = 0, nb_par=0;
  int ni = blocs[0].dim;
  particule *tmp;
  blocs[ind].ps = malloc(sizeof(particule)*blocs[ind].dim);
  tmp = blocs[ind].ps;
  while ( fgets(ligne, TAILLE_LIGNE, f) != NULL ){
    if (nb_par == ni){
      ind++;
      blocs[ind].dim = atoi(ligne);
      blocs[ind].ps = malloc(sizeof(particule)*blocs[ind].dim);
      tmp = blocs[ind].ps;
      ni = blocs[ind].dim;
      nb_par = 0;
      continue;
    }
    char *token;
    char delim[] = " ";
    token = strtok(ligne, delim);
    tmp->m = atoi(token);
    token = strtok(NULL, delim);
    tmp->p.x = atof(token);
    token = strtok(NULL, delim);
    tmp->p.y = atof(token);
    token = strtok(NULL, delim);
    tmp->v.x = atof(token);
    token = strtok(NULL, delim);
    tmp->v.y = atof(token);
    tmp->a.x = 0.0;
    tmp->a.y = 0.0;
    tmp->f_ext.x = 0.0;
    tmp->f_ext.y = 0.0;
    tmp->proche_d = 0.0;
    tmp->id_bloc = ind;
    ++tmp;
    nb_par++;
  }
  fclose(f);
}

void process_interaction_bloc(bloc *a, bloc *b, float bloc_size, int lock){  
  vecteur force_tmp;
  float s = sqrt(2)*bloc_size;
  for (int n = 0; n < a->dim; n++){  
    if (s/distance(&(a->ps[n]),&(b->center)) < THETA){
      force_grav(&(b->center), &(a->ps[n]), &force_tmp);
      somme(&((a->ps[n]).f_ext),&(force_tmp),&((a->ps[n]).f_ext));
    }
 
    else {
      for (int p = 0; p < b->dim; p++){
	if ( (n == p) && lock){
	  continue;
	}	

	double dist = distance(&(a->ps[n]),&(b->ps[p]));
	if ((a->ps[n]).proche_d == 0.0 ||
	    dist < (a->ps[n]).proche_d){
	  (a->ps[n]).proche_d = dist;
	}
	force_grav(&(b->ps[p]), &(a->ps[n]), &force_tmp);
	somme(&((a->ps[n]).f_ext),&(force_tmp),&((a->ps[n]).f_ext));
      
 }
    }
  }
}




void masse_center(bloc *b){
  if(b->dim == 0){
    puts("Problem dim");
    exit(EXIT_FAILURE);
  }
  (b->center).p.x = 0;
  (b->center).p.y = 0;
  (b->center).m = 0;
  for (int i = 0; i < b->dim; i++){
    (b->center).p.x += (b->ps[i]).m * (b->ps[i]).p.x;
    (b->center).p.y += (b->ps[i]).m * (b->ps[i]).p.y;
    (b->center).m += (b->ps[i]).m;
  }
  (b->center).p.x /= (b->center).m;
  (b->center).p.y /= (b->center).m;  
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
    ps->f_ext.x = 0.0;
    ps->f_ext.y = 0.0;
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
  int alpha = nb/size;
  while (i < nb && j < alpha){
    fgets(ligne,MAX,fd);
    if (i < (rank+1)*alpha && (i >= rank*alpha)){
      sscanf(ligne, "%d %lf %lf %lf %lf", &((univers+j)->m),&((univers+j)->p.x),
      	     &((univers+j)->p.y), &((univers+j)->v.x),&((univers+j)->v.y));
      (univers+j)->a.x = 0.0;
      (univers+j)->a.y = 0.0;
      (univers+j)->proche_d = 0.0;
      (univers+j)->f_ext.x = 0.0;
      (univers+j)->f_ext.y = 0.0;
      j++;
    }
    i++;
  }
  fclose(fd);
}


/* Faire une function qui transforme une ligne du fichier en particule */
void parse_par_bloc(char* filename, particule* univers){  
  int nb,local_nb; char ligne[MAX];  
  FILE* fd = fopen(filename, "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    exit(EXIT_FAILURE);
  }
  /* number of particules */
  fgets(ligne, MAX, fd);
  nb = atoi(ligne);
  int size,rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(nb%size != 0){
    dprintf(stderr,"Number of particule should be divisible by np\n");
    exit(EXIT_FAILURE);
  }
  local_nb = nb/size;
  /* placing read cursor */
  for(int i=0;i<rank*(local_nb+1)+1;++i)
    fgets(ligne, MAX, fd);
  /*reading local particules */
  for(int i=0;i<local_nb;++i){
    fgets(ligne,MAX,fd);
    sscanf(ligne, "%d %lf %lf %lf %lf", &((univers+i)->m),&((univers+i)->p.x),&((univers+i)->p.y), &((univers+i)->v.x),&((univers+i)->v.y));
    (univers+i)->a.x = 0.0;
    (univers+i)->a.y = 0.0;
    (univers+i)->proche_d = 0.0;
    (univers+i)->f_ext.x = 0.0;
    (univers+i)->f_ext.y = 0.0;
  }      
  fclose(fd);
}

#endif 

