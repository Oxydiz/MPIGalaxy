#ifndef INC_TYPES
#define INC_TYPES

//Parameters
#define WIDTH 800
#define HEIGHT 600
#define CLIP_PLANE_X 400
#define CLIP_PLANE_Y 400/(WIDTH/HEIGHT)
#define PIXELSIZE 0.01f

//Error codes
#define WRONG_USAGE -1
#define UNSUPPORTED -2
#define FOPEN_FAIL -3

//Types
typedef struct Star {
  float x;
  float y;
  int m;
} Star;

//Global variables

#endif
