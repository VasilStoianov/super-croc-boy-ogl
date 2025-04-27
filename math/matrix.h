#ifndef MATRIX_4F
#define MATRIX_4F
#include "vector.h"
#include <string.h>

typedef struct {
  float m[4][4];
} mat4f;

mat4f identity() {
  mat4f res = {0};
  memset(res.m, 0.f, sizeof(mat4f));
  res.m[0][0] = res.m[1][1] = res.m[2][2] = res.m[3][3] = 1.f;

  return res;
}

void setTranslation(mat4f *matrix, vector vec) {

  matrix->m[0][3] = vec.x;
  matrix->m[1][3] = vec.y;
  matrix->m[2][3] = vec.z;
}

void scaleMatrix(mat4f *matrix, vector scale) {
  matrix->m[0][0] = scale.x;
  matrix->m[1][1] = scale.y;
  matrix->m[2][2] = scale.z;
}
/*
orthographic matrix making 0,0 bot left;
|  2/(right-left)       0                  0    -(right+left)/(right-left) |
|  0                  2/(top-bottom)        0    -(top+bottom)/(top-bottom) |
|  0                  0                 -2/(far-near) -(far+near)/(far-near)|
|  0                  0                  0                 1               |
*/

mat4f ortho(float left, float width, float top, float heidht, float near,
            float far) {
  mat4f ort = identity();
  ort.m[0][0] = 2.f / (width - left);
  ort.m[1][1] = 2.f / (top - heidht);
  ort.m[2][2] = -2.f / (far - near);
  ort.m[3][3] = 1.f;
  ort.m[0][3] = -(width + left) / (width - left);
  ort.m[1][3] = -(top + heidht) / (top - heidht);
  ort.m[2][3] = -(far + near) / (far - near);
  return ort;
}

void mat4_ortho(mat4f *out,
                 float left,   float right,
                 float bottom, float top,
                 float near,   float far)
{
    // Zero‐initialize all elements
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            out->m[i][j] = 0.0f;

    // Scale X and Y into the −1…+1 range
    out->m[0][0] =  2.0f / (right - left);
    out->m[1][1] =  2.0f / (top   - bottom);
    // Scale (and flip) Z
    out->m[2][2] = -2.0f / (far   -  near);

    // Translate X and Y (and Z) so that
    // world = (left, bottom, near) → NDC = (−1,−1,−1)
    // world = (right, top, far)   → NDC = (+1,+1,+1)
    out->m[3][0] = -(right + left)   / (right - left);
    out->m[3][1] = -(top   + bottom) / (top   - bottom);
    out->m[3][2] = -(far   +  near)  / (far   -  near);

    // Bottom-right element stays 1
    out->m[3][3] = 1.0f;
}

mat4f ortho_top_left(float width, float height)
{
    mat4f M = identity();

    M.m[0][0] =  2.0f / width;   // X-scale
    M.m[1][1] = 2.0f / height;  // Y-scale (note minus)
    M.m[2][2] = 1.0f;           // Z-scale for 2-D
    M.m[0][3] = -1.0f;           // shift X so 0 → -1
    M.m[1][3] =  1.0f;           // shift Y so 0 → +1
    return M;
}
#endif