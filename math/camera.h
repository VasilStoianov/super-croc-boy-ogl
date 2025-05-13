#ifndef __CAMERA__
#define __CAMERA__
#include "matrix.h"
#include "vector.h"
#include "../shaders/shader.h"
#include <stdbool.h>

typedef struct {
  mat4f matrix;
  mat4f shake_matrix;
  vector position;
  vector shake;
  float shakeDuration;
  float shakeTime;
  float shakeStregth; 
  bool startShaking;
} Camera;

Camera *create_camera() {
  Camera *camera = (Camera *)malloc(sizeof(Camera));
  camera->matrix = identity();
  camera->shake_matrix = identity();
  camera->shakeDuration = 1.0f;
  camera->shakeStregth = 8.f;
  camera->shakeTime = 0.0f;
  camera->position = (vector){.x = 0.f, .y = 0.f};
  return camera;
}

void set_camera(Camera *camera, int shaderId) {

  setTranslation(&(camera->matrix), camera->position);
  set_matrix_uniform(camera->matrix, shaderId, "camera");
}

void set_camera_shake(Camera *camera, int shaderId) {

  setTranslation(&(camera->shake_matrix), camera->shake);
  set_matrix_uniform(camera->shake_matrix, shaderId, "shake");
}

void shake_camera(Camera *camera, float dt,float shaderId){
 
    if(camera->startShaking){
    if(camera->shakeTime<camera->shakeDuration){
        camera->shakeTime += dt;
    }else {
        camera->shakeTime = 0 ;
        camera->startShaking = 0;
    }

    float remaining = camera->shakeDuration - camera->shakeTime;
    float decay = remaining / camera->shakeDuration;

    camera->shake.x= ((rand() %2001-1000) / 1000.f) * camera->shakeStregth * decay;
    camera->shake.y= ((rand() %2001-1000) / 1000.f) * camera->shakeStregth * decay;

  set_camera_shake(camera, shaderId);
    }else {
        camera->shake = createZeroVector();
        camera->shakeTime = 0.f;
  set_camera_shake(camera, shaderId);
    }

}

#endif