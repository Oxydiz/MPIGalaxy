#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <mpi.h>
#include "hacked_mpi.h"
#include "definitions.hpp"
#include "iofunc.hpp"
#include "ogl.hpp"

void moveGalaxy(Star *galaxy, int nbStars) {

  int i,j;
  long double dx, dy, dst, Fx = 0, Fy = 0, ax, ay;

  for(i = 0; i < nbStars; i++) {
    for(j = 0; j < nbStars; j++) {
      if(i == j) continue;
      //We calculate the direction vector;
      dx = galaxy[j].x - galaxy[i].x;
      dy = galaxy[j].y - galaxy[i].y;
      dst = dx * dx + dy * dy;
      //We calculate the acceleration
      Fx += (CST_G * ((double) galaxy[i].m * galaxy[j].m) / dst) * dx;
      Fy += (CST_G * ((double) galaxy[i].m * galaxy[j].m) / dst) * dy;
      // Fx += (CST_G * ((double) 1) / dst) * dx;
      // Fy += (CST_G * ((double) 1) / dst) * dy;
    }
    ax = Fx / ((double) 1);
    ay = Fy / ((double) 1);
    if(i == 0)
      printf("s : [%f,%f,%d] f : [%Lf,%Lf] a : [%Lf,%Lf]\n",galaxy[i].x,galaxy[i].y,galaxy[i].m,Fx,Fy,ax,ay);
    //We update the speed
    galaxy[i].sx += ax * DELTA_T;
    galaxy[i].sy += ay * DELTA_T;
    //We update the position
    galaxy[i].x += galaxy[i].sx * DELTA_T;
    galaxy[i].y += galaxy[i].sy * DELTA_T;
    Fx = Fy = 0;
  }
}

void moveGalaxyOld(Star *galaxy, int nbStars) {

  int i;
  double x = 0, y = 0, cx, cy, a, dx, dy, dst; //c = corrected, a = acceleration, d = direction
  long mass = 0, cmass;
  for(i = 0; i < nbStars; i++) {
    x+=galaxy[i].x;
    y+=galaxy[i].y;
    mass++;
  }

  x/=nbStars;
  y/=nbStars;

  for(i = 0; i < nbStars; i++) {
    //We correct the global mass and position
    cx = x - (galaxy[i].x / nbStars);
    cy = y - (galaxy[i].y / nbStars);
    cmass = mass - 1;
    //We calculate the direction vector;
    dx = cx - galaxy[i].x;
    dy = cy - galaxy[i].y;
    dst = dx * dx + dy * dy;
    //We normalize the direction vector
    dx /= dst;
    dy /= dst;
    //We calculate the acceleration
    a = ((double)cmass) * CST_G * dst;
    if(i == 0)
      printf("s : [%f,%f,%d] c : [%f,%f,%ld], d : [%f,%f,%f] a : %f\n",galaxy[i].x,galaxy[i].y,galaxy[i].m,cx,cy,cmass,dx,dy,sqrt(dst),a);
    //We update the speed
    galaxy[i].sx += a * dx * DELTA_T;
    galaxy[i].sy += a * dy * DELTA_T;
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
  Star *galaxy = loadGalaxy(v[1], &nbStars,NULL);

  FILE *f = initStorage(v[2],nbStars, nbIterations);
  storeGalaxy(f,galaxy,nbStars);

  int i;
  for(i = 0; i < nbIterations; i++) {
    moveGalaxy(galaxy,nbStars);
    storeGalaxy(f,galaxy,nbStars);
  }

  free(galaxy);
  MPI_Finalize();
  return EXIT_SUCCESS;

}
