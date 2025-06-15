#ifndef UTILS_H
#define UTILS_H
#include "math/vector.h"
#include <stdio.h>

#define COLOR_BLACK ((vector){.x = 0,.y = 0,.z=0})
#define COLOR_WHITE ((vector){255,255,255})
#define COLOR_RED   ((vector){255,0,0})
#define COLOR_LIME ((vector){0,255,0})
#define COLOR_BLUE ((vector){0,0,255})
#define COLOR_YELOOW ((vector){255,255,0}) 
#define COLOR_GRAY ((vector){128,128,128}) 

float dt = 1.0 / 60.f;

float clamp(float value, float min, float max) {
  if (value > max)
    return max;
  if (value < min)
    return min;
  return value;
}


void print_matrix(mat4f matrix){
    for(int x = 0; x<4; x++){
        for(int y = 0; y<4; y++){
            printf("%f ", matrix.m[x][y]);
        }
        printf("\n");
    }
}


#endif