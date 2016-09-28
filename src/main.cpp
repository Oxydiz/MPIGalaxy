#include <stdlib.h>
#include <stdio.h>
//#include <mpi.h>
#include "hacked_mpi.h"
#include "types.hpp"
#include "iofunc.hpp"

int main(int c,char **v) {

  if(c != 2) { printf("Usage : program [InputFile].\n"); exit(-1); }

  MPI_Init(&c,&v);

  int id, size, nbStars;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  Star *galaxy = loadGalaxy(v[1], &nbStars);
  printf("%f",galaxy[3].y);

  free(galaxy);
  MPI_Finalize();
  return EXIT_SUCCESS;

}
