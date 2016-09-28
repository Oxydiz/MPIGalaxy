#include <stdlib.h>
#include <stdio.h>
//#include <mpi.h>
#include "hacked_mpi.h"
#include "definitions.hpp"
#include "iofunc.hpp"

int main(int c,char **v) {

  if(c != 4) { printf("Usage : program [InputFile] [OutputFile] [Iterations].\n"); exit(WRONG_USAGE); }

  MPI_Init(&c,&v);

  int id, size, nbStars;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  Star *galaxy = loadGalaxy(v[1], &nbStars);

  FILE *f = initStorage(v[2],nbStars);
  storeGalaxy(f,galaxy,nbStars);
  storeGalaxy(f,galaxy,nbStars);

  free(galaxy);
  MPI_Finalize();
  return EXIT_SUCCESS;

}
