#include <stdio.h>
#include <util.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <particule.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NB_ITERATIONS 2

int main(int argc, char** argv){
  
  
  if (argc != 3) {
    printf("Erreur : Argument manquant\n");
    printf("Usage : ./exec particules_file output_file\n");
    return EXIT_FAILURE;
  } 

  MPI_Init(NULL,NULL);
  int rank, size, tag;
  int root = 0;
  tag = 99;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  /* fichier d'ouput des résultats */
  
  int output = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC,0755);
  if (!output){
    perror("open : fichier output\n");
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  /* fichier des particules */ 
  FILE* fd = fopen(argv[1], "r");
  if (!fd){
    perror("Erreur ouverture fichier\n");
    exit(EXIT_FAILURE);
  }
  

  char ligne[MAX];
  fgets(ligne, MAX, fd);
  int m = atoi(ligne);
  int alpha = m/size; /* nombre de particules par processeurs */
  particule univers[alpha]; 

  
  if ( m % size != 0){
    printf("Nombre de processeurs doit diviser le nombre de particules initial\n");
    MPI_Finalize();
    return EXIT_FAILURE;
  }
  /* initialisation des particules à partir du fichier */
  parse_particule_par(argv[1], rank, univers);
  MPI_Status status;
  MPI_Request request, request2;
  
  // array composed of send data and received data
  particule *send = malloc(sizeof(particule)*alpha); 
  particule *recv = malloc(sizeof(particule)*alpha);
  particule *tmp;
  double dt = 0.0;
  double t = 0.0;
  int i, j, k,n,p;
  vecteur force_tmp;
  MPI_Datatype Particule_d, Vecteur_d, Transport_d;
  /* MPI_Datatype du vecteur */
  
  int blocklengths[2] = {1,1};
  MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
  MPI_Aint offsets[2];
  offsets[0] = offsetof(vecteur,x);
  offsets[1] = offsetof(vecteur,y);
  MPI_Type_create_struct(2,blocklengths,offsets,types,&Vecteur_d);
  MPI_Type_commit(&Vecteur_d);
  
  /* MPI_Datatype d'une particule (sous particule seulement les infos necessaires */
  int blocklengths_p[2] = {1,1};
  MPI_Datatype types_p[2] =  {MPI_INT, Vecteur_d};
  MPI_Aint offsets_p[2];
  offsets_p[0] = offsetof(particule,m);
  offsets_p[1] = offsetof(particule,p);
  MPI_Type_create_struct(2,blocklengths_p,offsets_p,types_p,&Particule_d);
  MPI_Type_commit(&Particule_d);
  MPI_Type_create_resized(Particule_d,offsets_p[0],sizeof(particule),&Transport_d);
  /* if(rank == 1){ */
  /*   for(int cmp=0; cmp<alpha; ++cmp) */
  /*     print_particule(univers+cmp); */
  /*   fflush(stdout); */
  /* } */

  /* if(rank == 1){ */
  /*   MPI_Send(univers,alpha,Transport_d,0,tag,MPI_COMM_WORLD); */
  /*   //MPI_Wait(&request,&status); */
      
  /* } */
  /* if(rank == 0){ */
  /*   MPI_Recv(recv,alpha,Transport_d,1,tag,MPI_COMM_WORLD,&status); */
  /*   //MPI_Wait(&request2,&status); */
  /* } */
  /* if(rank == 0){ */
  /*   for(int cmp=0; cmp<alpha; ++cmp) */
  /*     print_particule(recv+cmp); */
  /*   fflush(stdout); */
  /* } */

  i = 0; j = 0; k = 0; n = 0; p = 0;
  while(i < NB_ITERATIONS) {
    j = 0;
    /* deplace nos particules dans le buffer d'envoi */
    memcpy(send, univers, sizeof(particule)*alpha);
    while (j < size){
      /* pour ne pas s'envoyer à soit même */
      /* printf("ROUND %d, I AM  %d, SENDING TO %d, RECEIVING FROM %d\n", j,rank,(rank-j-1+size) % size, (rank+j+1)%size); */
      /* fflush(stdout); */
  	MPI_Isend(send,alpha,Transport_d,(rank-1+size)%size,tag,MPI_COMM_WORLD,&request);
  	MPI_Irecv(recv,alpha,Transport_d,(rank+1)%size, tag, MPI_COMM_WORLD,&request2);
      // les commandes MPI ne sont peut-être pas enclenchées
      // get the nearest particule in the whole universe
	if(rank == 0){
	  puts("###############\n");
	  for(int cmp=0; cmp<alpha; ++cmp)
	    print_particule(univers+cmp);
	  fflush(stdout);
	}
	  while (n < alpha){
  	while (p < alpha){
  	  if (n == p && j == 0){
  	    p++;
  	    continue;
  	  }

  	  // calcul de la distance de la particule la plus proche
  	  double dist = distance(&send[p], &univers[n]);
  	  if (equal_double(univers[n].proche_d,0.0) ||
  	      dist < univers[n].proche_d){
  	    univers[n].proche_d = dist;
  	  }
  	  /* calcul des forces */
  	  force_grav(&send[p], &univers[n], &force_tmp);
  	  somme(&(univers[n].f_ext),&(force_tmp),&(univers[n].f_ext));
  	  p++;

  	}
  	p=0;
  	n++;
      }
      n = 0;
      //swap send et received buffers
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Wait(&request,&status);
      MPI_Wait(&request2,&status);
      
      tmp = send;
      send = recv;
      recv = tmp;
      MPI_Barrier(MPI_COMM_WORLD);
      j++;
    }
    dt = nouveau_dt(univers,alpha);
    MPI_Allreduce(&dt,&dt,1,MPI_DOUBLE,MPI_MIN,MPI_COMM_WORLD);
    update_particules(univers, alpha, dt);
      
    t += dt;
     
    //MPI_Barrier(MPI_COMM_WORLD);
    //MPI_Gather(univers,alpha,Particule_d,galaxy,alpha,Particule_d,root,MPI_COMM_WORLD);
    log_particules_par(univers,alpha,output,t,root,i);
    i++;
  }
  
  MPI_Finalize();
  free(send); free(recv);
  close(output);
  return 0;
}
