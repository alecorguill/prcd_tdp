#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include <util.h>
#include <life.h>

int BS;

int main(int argc, char* argv[])
{
  int i, j, loop, num_alive, num_alive_loc, maxloop;
  int ldboard, ldnbngb,ldboard_loc, ldnbngb_loc;
  double t1, t2;
  double temps,temps_loc;

  int *board, *board_loc;
  int *nbngb, *nbngb_loc;

  if (argc < 3) {
    printf("Usage: %s nb_iterations size\n", argv[0]);
    return EXIT_SUCCESS;
  } else {
    maxloop = atoi(argv[1]);
    BS = atoi(argv[2]);
    //printf("Running sequential version, grid of size %d, %d iterations\n", BS, maxloop);
  }
  MPI_Init(NULL,NULL);
  int rank,size,tag,root;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  root = 0;
  //
  int nb_bloc = (int) sqrt(size);
  
  if (BS % nb_bloc != 0){
    printf("Usage: Wrong size and number of processes; sqrt(np) must divide size");
    MPI_Finalize();
    return EXIT_SUCCESS;
  }
      
  /* Creating grid */
  MPI_Comm grid;
  int ndims = 2; int reorder=1;
  int dims[] = {nb_bloc,nb_bloc};
  int periods[] = {1,1};
  MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,reorder,&grid);
  int bbs = BS/nb_bloc;
  
  num_alive = 0;
  num_alive_loc = 0;
 
  /* Leading dimension of the board array */
  ldboard = BS + 2;
  /* Leading dimension of the neigbour counters array */
  ldnbngb = BS;
  ldboard_loc = bbs + 2;
  ldnbngb_loc = bbs;
 
  //block datatype
  MPI_Datatype bloc,bloc_loc,bloc_resized,bloc_loc_resized,row_loc;
  MPI_Type_vector(bbs,bbs,ldboard,MPI_INT,&bloc);
  MPI_Type_vector(bbs,bbs,ldboard_loc,MPI_INT,&bloc_loc);
  MPI_Type_vector(ldboard_loc,1,ldboard_loc,MPI_INT,&row_loc);
  MPI_Type_commit(&bloc);
  MPI_Type_commit(&bloc_loc);
  MPI_Type_commit(&row_loc);
  MPI_Type_create_resized(bloc,(MPI_Aint) 0, (MPI_Aint) bbs*sizeof(int),&bloc_resized);
  MPI_Type_create_resized(bloc_loc,(MPI_Aint) 0, (MPI_Aint) bbs*sizeof(int),&bloc_loc_resized);
  MPI_Type_commit(&bloc_loc_resized);
  MPI_Type_commit(&bloc_resized);

  if (rank == root){
    board = calloc(sizeof(int), ldboard * ldboard );
    nbngb = calloc(sizeof(int), ldnbngb * ldnbngb);
    num_alive = generate_initial_board( BS, &(cell(1,1,board,ldboard)), ldboard );
  }
  board_loc = calloc(sizeof(int),ldboard_loc * ldboard_loc);
  nbngb_loc = calloc(sizeof(int),ldnbngb_loc * ldnbngb_loc);
 
  int sendcounts[size],displs[size];
  int dis = 0;
  int coord[2];
  for (int i=0; i < size; i++){
    sendcounts[i] = 1;
    MPI_Cart_coords(grid,i,2,coord);
    displs[i] = coord[0]+coord[1]*ldboard;
  }

  MPI_Scatterv(&(cell(1,1,board,ldboard)),sendcounts,displs,bloc_resized,&(cell(1, 1, board_loc, ldboard_loc)),1,bloc_loc,root,MPI_COMM_WORLD);
 
  if (rank == root){
    printf("Starting number of living cells = %d\n", num_alive);
    output_board( BS, &(cell(1, 1,board,ldboard)), ldboard, rank);
 }
    
  MPI_Aint lb, extent;
  MPI_Type_get_extent(row_loc,&lb,&extent);
  if (rank == root)
    printf("%d %d\n", lb,extent);
  MPI_Status status;
  t1 = mytimer();
  for (loop = 1; loop <= maxloop; loop++) {
        
    // Transmission des bords
    int trank,brank,lrank,rrank;
    MPI_Cart_shift(grid,0,1,&lrank,&rrank);
    MPI_Cart_shift(grid,1,1,&trank,&brank);
    MPI_Sendrecv(&cell(1, bbs, board_loc, ldboard_loc),ldboard_loc,MPI_INT,rrank,99,&cell(1,0, board_loc, ldboard_loc),ldboard_loc,MPI_INT,rrank,99,MPI_COMM_WORLD,&status);
    MPI_Sendrecv(&cell(1,1, board_loc, ldboard_loc),ldboard_loc,MPI_INT,lrank,99,&cell(1,bbs+1, board_loc, ldboard_loc),ldboard_loc,MPI_INT,lrank,99,MPI_COMM_WORLD,&status);
    MPI_Sendrecv(&cell(1, 0, board_loc, ldboard_loc),ldboard_loc,row_loc,trank,99,&cell(bbs+1,0, board_loc, ldboard_loc),ldboard_loc,row_loc,trank,99,MPI_COMM_WORLD,&status);
    MPI_Sendrecv(&cell(bbs, 0, board_loc, ldboard_loc),ldboard_loc,row_loc,brank,99,&cell(0,0, board_loc, ldboard_loc),ldboard_loc,row_loc,brank,99,MPI_COMM_WORLD,&status);
 
  
    for (j = 1; j <= bbs; j++) {
      for (i = 1; i <= bbs; i++) {
  	ngb( i, j,board,ldboard) =
  	  cell(i-1, j-1,board_loc,ldboard_loc) + cell(i,j-1,board_loc,ldboard_loc) + cell(i+1,j-1,board_loc,ldboard_loc) +
  	  cell( i-1, j, board_loc, ldboard_loc) + cell(i+1, j,board_loc,ldboard_loc) +
  	  cell(i-1,j+1,board_loc,ldboard_loc) + cell(i,j+1,board_loc,ldboard_loc) + cell(i+1,j+1,board_loc,ldboard_loc);
      }
    }

    num_alive_loc = 0;
    for (j = 1; j <= bbs; j++) {
      for (i = 1; i <= bbs; i++) {
  	if ( (ngb( i, j,board_loc,ldboard_loc) < 2) ||
  	     (ngb( i, j,board_loc,ldboard_loc) > 3) ) {
  	  cell(i, j,board_loc,ldboard_loc) = 0;
  	}
  	else {
  	  if ((ngb( i, j,board_loc,ldboard_loc )) == 3)
  	    cell(i, j,board_loc,ldboard_loc) = 1;
  	}
  	if (cell(i,j,board_loc,ldboard_loc) == 1) {
  	  num_alive_loc ++;
  	}
      }
    }
  
  

    /* Avec les celluls sur les bords (utile pour vérifier les comm MPI) */
    /* output_board( BS+2, &(cell(0, 0)), ldboard, loop ); */

    /* Avec juste les "vraies" cellules: on commence à l'élément (1,1) */
    /*output_board( BS, &(cell(1, 1)), ldboard, loop);*/

    //printf("Rank %d : %d cells are alive\n",rank,num_alive_loc);
  }

  t2 = mytimer();
  temps_loc = t2 - t1;
  MPI_Reduce(&temps,&temps_loc,1,MPI_DOUBLE,MPI_MAX,root,MPI_COMM_WORLD);
  MPI_Reduce(&num_alive,&num_alive_loc,1,MPI_DOUBLE,MPI_SUM,root,MPI_COMM_WORLD);
  
  int recvcounts[size],recvdispls[size];
  for (int i=0; i < size; i++){
    recvcounts[i] = 1;
    MPI_Cart_coords(grid,i,2,coord);
    recvdispls[i] = coord[0]+coord[1]*ldboard;
  }

  MPI_Gatherv(&(cell(1,1,board_loc,ldboard_loc)),1,bloc_loc,&(cell(1, 1,board,ldboard)),recvcounts,recvdispls,bloc_resized,root,MPI_COMM_WORLD);

  if (rank == root){
    output_board(ldboard,&(cell(0, 0,board,ldboard)),ldboard,0);
    printf("Final number of living cells = %d\n", num_alive);
    printf("%.2lf\n",(double)temps * 1.e3);
  }

  if (rank == 0){
    free(board);
    free(nbngb);
  }
  free(board_loc);
  free(nbngb_loc);
  MPI_Finalize();
  return EXIT_SUCCESS;
}

