#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <mpi.h>
#include "hacked_mpi.h"
#include "definitions.hpp"
#include "iofunc.hpp"
#include "ogl.hpp"

#include <omp.h>

MPI_Datatype mpi_star;

void moveGalaxy(Star *galaxy, int nbStars, int id, int split) {

  int i,j;
  long double dx, dy, dst, Fx = 0, Fy = 0, ax, ay;

  //#pragma omp parallel for private(j,dx,dy,dst,Fx,Fy,ax,ay)
  for(i = id * split; i < (id + 1) * split; i++) {
    if(i>=nbStars) {  //Breaks the loop if i >= nbStars. This is a work around omp for limitations
      i+=nbStars;
      continue;
    }
    for(j = 0; j < nbStars; j++) {
      if(i == j) continue;
      //We calculate the direction vector;
      dx = galaxy[j].x - galaxy[i].x;
      dy = galaxy[j].y - galaxy[i].y;
      dst = dx * dx + dy * dy;
      //We calculate the acceleration
      Fx += (((double)galaxy[j].m) / dst) * dx;
      Fy += (((double)galaxy[j].m) / dst) * dy;
    }
    ax = CST_G * Fx;
    ay = CST_G * Fy;

    //We update the speed
    galaxy[i].sx += ax * DELTA_T;
    galaxy[i].sy += ay * DELTA_T;
    Fx = Fy = 0;
  }

  #pragma omp parallel for
  for(i = id * split; i < (id + 1) * split; i++) {
    if(i>=nbStars) {  //Breaks the loop if i >= nbStars. This is a work around omp for limitations
      i+=nbStars;
      continue;
    }
    //We update the position
    galaxy[i].x += galaxy[i].sx * DELTA_T;
    galaxy[i].y += galaxy[i].sy * DELTA_T;
  }

}

int main(int c,char **v) {

  if(c != 4 && c != 2) { printf("Usage : program [InputFile] [OutputFile] [Iterations] OR program [OpenGLInputFile].\n"); exit(WRONG_USAGE); }

  if(c == 2)
    return openGLMode(c,v);

  int nbIterations = atoi(v[3]);
  if(nbIterations < 1) { printf("Invalid number of iterations.\n"); exit(UNSUPPORTED); }

  MPI_Init(&c,&v);
  int id, size, nbStars;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);

  //We define in MPI the new type
  MPI_Datatype types[2] = {MPI_INT, MPI_FLOAT};
  int len[2] = {1,4};
  MPI_Aint offset[2] = {(MPI_Aint)offsetof(Star, m),(MPI_Aint)offsetof(Star, x)};
  MPI_Type_create_struct(2, len, offset, types, &mpi_star);
  MPI_Type_commit(&mpi_star);

  Star *galaxy = NULL;
  FILE *f = NULL;

  if(id == 0) {
    galaxy = loadGalaxy(v[1], &nbStars,NULL);

    //We send the necessary data
    MPI_Bcast(&nbStars,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(galaxy,nbStars,mpi_star,0,MPI_COMM_WORLD);

    //We store the intial state of the galaxy
    f = initStorage(v[2],nbStars, nbIterations);
    storeGalaxy(f,galaxy,nbStars);

  } else {
    //Receive the necessary data
    MPI_Bcast(&nbStars,1,MPI_INT,0,MPI_COMM_WORLD);
    galaxy = (Star*)malloc(nbStars * sizeof(Star));
    MPI_Bcast(galaxy,nbStars,mpi_star,0,MPI_COMM_WORLD);
  }

  int i;

  //Single processor case
  if(size == 1) {
    for(i = 0; i < nbIterations; i++) {
      moveGalaxy(galaxy,nbStars,0,nbStars);
      storeGalaxy(f,galaxy,nbStars);
    }
  } else {
    //Multiple processor case. Processor 0 is responsible for the output, all other processors compute

    //Here we set up the communication so that processor 0 listens to AllGatherv without sending anything
    int *recv = (int*)malloc(size * sizeof(int)), *off = (int*)malloc(size * sizeof(int)), split = nbStars / (size - 1);
    recv[0] = 0;
    off[0] = -split;
    for(i = 1; i < size; i++) {
      recv[i] = split;
      off[i] = off[i - 1] + split;
    }
    off[0] = 0;

    //Processor 0 receives the consecutive states of the galaxy and outputs them
    if(id == 0) {
      for(i = 0; i < nbIterations; i++) {
        MPI_Allgatherv(galaxy,recv[0],mpi_star,galaxy,recv,off,mpi_star,MPI_COMM_WORLD);
        storeGalaxy(f,galaxy,nbStars);
      }
      double t;
      MPI_Status status;
      MPI_Recv(&t,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD,&status);
      printf("\nProcessing time with %d processors : %fs\n\n",size,t);
    } else {
      //Other processors compute the next galaxy state
      double t = -MPI_Wtime();
      for(i = 0; i < nbIterations; i++) {
        moveGalaxy(galaxy,nbStars,id-1,split);
        if(id == 1)
          moveGalaxy(galaxy,nbStars,size-1,split);
        MPI_Allgatherv(&(galaxy[split * (id-1)]),recv[id],mpi_star,galaxy,recv,off,mpi_star,MPI_COMM_WORLD);
      }
      t+=MPI_Wtime();
      if(id == 1)
        MPI_Send(&t,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }

    free(recv);
    free(off);
  }

  free(galaxy);
  fclose(f);
  MPI_Finalize();
  return EXIT_SUCCESS;

}
