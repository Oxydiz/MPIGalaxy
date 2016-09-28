#include <stdlib.h>
#include <stdio.h>
//#include <mpi.h>
#include "hacked_mpi.h"
#include "definitions.hpp"
#include "iofunc.hpp"
#include "ogl.hpp"

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
  Star *galaxy = loadGalaxy(v[1], &nbStars,NULL);

  FILE *f = initStorage(v[2],nbStars, nbIterations);
  //TODO:Change this
  int i;
  for(i = 0; i < nbIterations; i++)
    storeGalaxy(f,galaxy,nbStars);

  free(galaxy);
  MPI_Finalize();
  return EXIT_SUCCESS;

}
