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

#define NB_ITERATIONS 20

int main(int argc, char** argv){

   ////---------------------------------------------------------////
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

    int output = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC,0744);
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
    bloc univers;

    if ( m % size != 0){
        printf("Nombre de processeurs doit diviser le nombre de particules initial\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    /* Initialisation des particules à partir du fichier */
    parse_particule_par(argv[1], rank, univers.ps);
    ////---------------------------------------------------------////
   
 
    
    ////---------------------------------------------------------////
    MPI_Datatype Particule_d, Vecteur_d, Transport_d;
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
    ////---------------------------------------------------------////


    
    MPI_Status status;
    MPI_Request request, request2, request3, request4;
    // Array composed of send data and received data
    particule *send = malloc(sizeof(particule)*alpha);
    particule *recv = malloc(sizeof(particule)*alpha);
    particule *center_send = malloc(sizeof(particule));
    particule *center_recv = malloc(sizeof(particule));
    particule *tmp;
    bloc tmp_interaction;
    double dt = 0.0;
    double t = 0.0;
    int k;
    vecteur force_tmp;
    
    masse_center(&univers);
    memcpy(send,univers.ps,sizeof(particule)*alpha);
    memcpy(center_send,univers.center,sizeof(particule));
    
    for (int j = 0; j < size; j++){
      MPI_Isend(send,alpha,Transport_d,(rank-1+size)%size,tag,MPI_COMM_WORLD,&request);
      MPI_Irecv(recv,alpha,Transport_d,(rank+1)%size,tag,MPI_COMM_WORLD,&request2);
      MPI_Isend(center_send,1,Transport_d,(rank-1+size)%size,tag,MPI_COMM_WORLD,&request3);
      MPI_Irecv(center_recv,1,Transport_d,(rank+1)%size,tag,MPI_COMM_WORLD,&request4);
    
      /**/
      tmp_interation.ps = send;
      tmp_interaction.center.p.x = center_send.p.x;
      tmp_interaction.center.p.y = center_send.p.y;
      tmp_interaction.center.p.m = center_send.p.m;
      process_interaction_bloc_par(univers,tmp_interaction,bloc_size);
      /**/

      // Swap send et received buffers
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Wait(&request,&status);
      MPI_Wait(&request2,&status);
      MPI_Wait(&request3,&status);
      MPI_Wait(&request4,&status);
      tmp = send;
      send = recv;
      recv = tmp;
      MPI_Barrier(MPI_COMM_WORLD);
  
      //log_particules_par(univers,alpha,output,t,root,i);
    }

    MPI_Finalize();
    free(send); free(recv);
    free(center_send); free(center_recv);
    close(output);
    return 0;
}
