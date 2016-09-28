#include "iofunc.hpp"

float parseFloat(FILE *f) {
  int i = 0;
  char c, str[20] = {'\0'};

  while((c = fgetc(f)) != EOF)
    switch(c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
        str[i++] = c;
        break;
      case ' ':
      case '\n':
      case 13: //CR
        return i == 0? -1 : atof(str);
      default:
        printf("Unsupported character : %c(%d)",c,c);
        exit(-2);

    }

  return 0;

}

Star *loadGalaxy(char *file, int *nbStars) {

  FILE *f = fopen(file,"r");
  if(f == NULL) { printf("Failed to open file.\n"); exit(-3); }

  *nbStars = (int)parseFloat(f);
  int i;
  Star *galaxy = (Star*)malloc(*nbStars * sizeof(Star));

  for(i = 0; i < *nbStars; i++) {
    parseFloat(f);
    parseFloat(f);
    galaxy[i].x = parseFloat(f);
    galaxy[i].y = parseFloat(f);
    galaxy[i].m = (int) parseFloat(f);
  }

  fclose(f);
  return galaxy;
}
