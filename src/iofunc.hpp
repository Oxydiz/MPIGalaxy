#ifndef INC_IOFUNC
#define INC_IOFUNC

#include <stdio.h>
#include <stdlib.h>
#include "definitions.hpp"

float parseFloat(FILE*);
Star *loadGalaxy(char*,int*);
FILE *initStorage(char*,int);
void storeGalaxy(FILE*,Star*,int);

#endif
