#ifndef __vertices__
#define __vertices__
#include "math.h"
#include <math.h>
const float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.5f,  0.5f, 0.0f,
  -0.5f,  0.5f, 0.0f
};

const float texture_vertices[] = {
  // x,     y,    z,    u,    v
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // Bottom Left
   0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // Bottom Right
   0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  // Top Right
  -0.5f,  0.5f, 0.0f, 0.0f, 0.0f   // Top Left
};
const unsigned int indices[] = {  
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 
 
const unsigned int indices2[] = {  
    0, 1, 2,   // first triangle
    2, 3, 0    // second triangle
}; 

float* circle_vertices(int cx,int cy,int radius,int segments,int* out_count){
  int count = segments + 2;
  float* vertices = malloc(sizeof(float) * 2 * count);

  vertices[0] = cx;
  vertices[1] = cy;

  for(int i = 0; i<=segments;++i){
      float angle = (2.f * M_PI * (float)i) /(float)segments;
      float x = cx + cosf(angle) *radius;
      float y = cy + sinf(angle) * radius;
      int x_index = 2 + i*2;
      int y_index = 2+i*2+1;
      vertices[x_index] = x;
      vertices[y_index] = y;
  }

  *out_count = count;
  return vertices;
}


float* circle_vertices_texture(int radius,int segments,int* out_count){
  int count = segments + 2;
  float* vertices = malloc(sizeof(float) * 4 * count);

  vertices[0] =0.0f;
  vertices[1] = 0.0f;
  vertices[2] = 0.5f;
  vertices[3] = 0.5f;

  for(int i = 0; i<=segments;++i){
      float angle = (2.f * M_PI * (float)i) /(float)segments;
      float x =  cosf(angle) *radius;
      float y = sinf(angle) * radius;
      int x_index = (i + 1) * 4;
      vertices[0 + x_index] = x;
      vertices[1 + x_index] = y;
      vertices[2 + x_index] = 0.5f + x / (2.0f * radius);
      vertices[3 + x_index] = 0.5f + y / (2.0f * radius);
  }

  *out_count = count;
  return vertices;
}

#endif
