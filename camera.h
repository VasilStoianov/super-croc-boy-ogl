#ifndef __CAMERA__
#define __CAMERA__
#include "math/matrix.h"
#include "math/vector.h"
#include "shader.h"

typedef struct {
  mat4f matrix;
  vector position;
} Camera;

Camera *create_camera() {
  Camera *camera = (Camera *)malloc(sizeof(Camera));
  camera->matrix = identity();
  camera->position = (vector){.x = 0.f, .y = 0.f};
  return camera;
}

void set_camera(Camera *camera, int shaderId) {

  setTranslation(&(camera->matrix), camera->position);
  set_matrix_uniform(camera->matrix, shaderId, "camera");
}

#endif