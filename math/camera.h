#ifndef __CAMERA__
#define __CAMERA__
#include "matrix.h"
#include "vector.h"
#include "../shaders/shader.h"
#include <stdbool.h>
#include "../game.h"

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



void set_camera(Camera* camera){
   set_camera_matrix(camera->matrix,camera->position);
}



void shake_camera(Camera *camera, float dt){
 
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

  set_camera_shake(camera->shake_matrix,camera->shake);
    }else {
        camera->shake = createZeroVector();
        camera->shakeTime = 0.f;
  set_camera_shake(camera->shake_matrix,camera->shake);
    }

}


void move_camera(Camera* camera,vector player_size,vector player_position, vector level_size,
                 float deltaTime) {
  float camPlayerLine =
      camera->position.x + player_position.x - player_size.x / 2.f;

  if (camPlayerLine > 600.f &&
      (camera->position.x * -1.f) + 800.f < level_size.x) {
    camera->position.x -= 850.f * deltaTime;
  } else if (camPlayerLine < 200.f && camera->position.x * -1.f > 0) {
    camera->position.x += 850.f * deltaTime;
  }
  setTranslation(&(camera->matrix),camera->position);
  set_camera_matrix(camera->matrix,camera->position);
  update_cam(camera->matrix);
  shake_camera(camera, deltaTime);
}

#endif