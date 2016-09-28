#ifndef INC_TYPES
#define INC_TYPES

#include <math.h>

//Parameters
#define WIDTH 800
#define HEIGHT 600
#define CLIP_PLANE_X 400
#define CLIP_PLANE_Y 400/(WIDTH/HEIGHT)
#define PIXELSIZE 0.01f
#define CST_G 6.674 * pow(10,-11) // 6.674 * 10^-11
#define DELTA_T 500

//Error codes
#define WRONG_USAGE -1
#define UNSUPPORTED -2
#define FOPEN_FAIL -3

//Types

typedef struct Star {
  float x;
  float y;
  int m;
  float sx;
  float sy;
} Star;

//Global variables

#endif
