#include <stdio.h>
#include <util.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <particule.h>
#include <stddef.h>

#define NB_ITERATIONS 1

int main(int argc, char** argv){
  
  if (argc != 3) {
    printf("Erreur : Argument manquant\n");
    printf("Usage : ./exec particules_file output_file\n");
    return EXIT_FAILURE;
  }

  FILE* output = fopen(argv[2], "w+");
  if (output == NULL){
    perror("fopen : fichier output\n");
    return EXIT_FAILURE;
  } 

  FILE* fd = fopen(argv[1], "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    exit(EXIT_FAILURE);
  }
  
  MPI_Init(NULL,NULL);
  int rank, size, tag;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char ligne[MAX];
  fgets(ligne, MAX, fd);
  int m = atoi(ligne);
  int alpha = m/size;
  particule  univers[alpha];
  int root = 0;
  
  if ( m % size != 0){
    printf("Nombre de processeurs doit diviser le nombre de particules initial\n");
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  
  // Configuration de l'univers
  parse_particule_par(argv[1], rank, univers);
 
  MPI_Status status;
  MPI_Request request, request2;
  
  // array composed of send data and received data
  // com[0] send, com[1] recv
  particule send[alpha], recv[alpha];
  
  init_buffers(alpha,send,univers);
  particule *tmp;
  double dt = 0.0;
  int i, j, k,n,p;
  vecteur force_tmp;
  MPI_Datatype Particule_d;
  MPI_Datatype Vecteur_d;
  int blocklengths[2] = {1,1};
  MPI_Datatype types[2] = {MPI_INT, MPI_INT};
  MPI_Aint offsets[2];
  offsets[0] = offsetof(vecteur,x);
  offsets[1] = offsetof(vecteur,y);
  MPI_Type_create_struct(2,blocklengths,offsets,types,&Vecteur_d);
  MPI_Type_commit(&Vecteur_d);
  
  int blocklengths_p[6] = {1,1,1,1,1,1};
  MPI_Datatype types_p[6] =  {MPI_INT, Vecteur_d, Vecteur_d,Vecteur_d,
  			    Vecteur_d, MPI_DOUBLE};
  MPI_Aint offsets_p[6];
  offsets[0] = offsetof(particule,m);
  offsets[1] = offsetof(particule,p);
  offsets[2] = offsetof(particule,v);
  offsets[3] = offsetof(particule,a);
  offsets[4] = offsetof(particule,f_ext);
  offsets[5] = offsetof(particule,proche_d);
  MPI_Type_create_struct(6,blocklengths_p,offsets_p,types_p,&Particule_d);
  MPI_Type_commit(&Particule_d);
  

  /* i = 0; j = 0; k = 0; n = 0; p = 0; */
  /* while(i < NB_ITERATIONS) { */
  /*   j = 0; */
  /*   while (j < size){ */
  /*     // recv */
  /*     if (rank == ((rank-j + size ) % size) || rank == ((rank+j) % size)){ */
  /* 	j++; */
  /* 	continue; */
  /*     } */
      
  /*     // send */
  /*     MPI_Isend(&recv,m,Particule_d,(rank-j + size ) % size,tag,MPI_COMM_WORLD,&request); */
  /*     MPI_Irecv(&send,m,Particule_d,(rank+j) % size, tag, MPI_COMM_WORLD,&request2); */
  /*     // les commandes MPI ne sont peut-être pas enclenchées */
  /*     // get the nearest particule in the whole universe */
  /*     while (n < m){ */
  /* 	while (p < m){ */
  /* 	  if (n == p && j == 0){ */
  /* 	    p++; */
  /* 	  continue; */
  /* 	  } */
  /* 	  // calcul de la distance de la particule la plus proche */
  /* 	  double dist = distance(&send[p], &univers[n]); */
  /* 	  //printf("distance : %lf\n", dist); */
  /* 	  if (univers[n].proche_d == 0.0 || */
  /* 	      dist < univers[n].proche_d){ */
  /* 	    univers[n].proche_d = dist; */
  /* 	  } */
  /* 	  force_grav(&send[p], &univers[n], &force_tmp); */
  /* 	  somme(&(univers[n].f_ext),&(force_tmp),&(univers[n].f_ext)); */
  /* 	  p++; */
  /* 	} */
  /* 	p=0; */
  /* 	n++; */
  /*     } */
  /*     n = 0; */

      // swap
      /* tmp = recv; */
      /* recv = send; */
      /* send = tmp; */
  /*     j++; */
  /*   } */

  /*   dt = nouveau_dt(univers,dt); */
  /*   MPI_Allreduce(&dt,&dt,1,MPI_DOUBLE,MPI_MIN,MPI_COMM_WORLD); */
  /*   update_particules(univers, m, dt); */
  /*   log_particules(univers, m, output); */
    
  /*   i++; */
  /* } */
  
  MPI_Finalize();
  fclose(output);
  return 0;
}
