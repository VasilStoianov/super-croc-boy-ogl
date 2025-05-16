#ifndef __player__
#define __player__
#include "../lib/glad.h"
#include "../input.h"
#include "../math/matrix.h"
#include "../math/vector.h"
#include "../shaders/shader.h"
#include "../utils.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>


typedef enum Direction { LEFT, RIGHT, UP, DOWN, STOP, LAST_DIR } Direction;

typedef struct Player {

  vector position;
  vector velocity;
  vector size;
  float maxSpeedX;
  float maxSpeedY;
  bool onGround;
  vector vertices[4];
  Direction action[LAST_DIR];
  mat4f translation;
  bool jump;

} Player;

Player *createPlayer() {

  Player *p = (Player *)malloc(sizeof(Player));

  p->velocity = createZeroVector();
  p->position = createZeroVector();
  p->maxSpeedX = 500.f;
  p->maxSpeedY = 450.f;
  p->translation = identity();
  p->onGround = false;
  p->jump = false;
  p->size = (vector){.x = 75.f, .y = 75.f, .z = 0.f};
  p->position.x = 600.f;
  p->position.y = 65.f ;

float hw = p->size.x * 0.5f;
float hh = p->size.y * 0.5f;

p->vertices[0] = (vector){p->position.x - hw, p->position.y - hh}; // top-left
p->vertices[1] = (vector){p->position.x + hw, p->position.y - hh}; // top-right
p->vertices[2] = (vector){p->position.x + hw, p->position.y + hh}; // bottom-right
p->vertices[3] = (vector){p->position.x - hw, p->position.y + hh}; // bottom-left
  return p;
}

void updatePlayer(Player *player, unsigned int prograId, float dt) {
  if (player->jump) {
    player->velocity.y -= 70.f;
    if (player->velocity.y < -player->maxSpeedY) {
      player->jump = false;
    }
  }

  player->position.x += player->velocity.x * dt;
  player->position.y += player->velocity.y * dt;
  
float hw = player->size.x * 0.5f;
float hh = player->size.y * 0.5f;

player->vertices[0] = (vector){player->position.x - hw, player->position.y - hh}; // top-left
player->vertices[1] = (vector){player->position.x + hw, player->position.y - hh}; // top-right
player->vertices[2] = (vector){player->position.x + hw, player->position.y + hh}; // bottom-right
player->vertices[3] = (vector){player->position.x - hw, player->position.y + hh}; // bottom-left
  setTranslation(&(player->translation), player->position);
  set_matrix_uniform(player->translation, prograId,"translation");
}

void scalePlayer(Player *player, vector scaleVec) {
  scaleMatrix(&(player->translation), scaleVec);
}
void set_player_action(Player *player) {

  player->action[LEFT] =
      input_key_held(GLFW_KEY_LEFT) || input_key_pressed(GLFW_KEY_LEFT);
  player->action[RIGHT] =
      input_key_held(GLFW_KEY_RIGHT) || input_key_pressed(GLFW_KEY_RIGHT);
  player->action[UP] = input_key_held(GLFW_KEY_UP) ||  input_key_pressed(GLFW_KEY_UP);
  player->action[DOWN] =
      input_key_held(GLFW_KEY_DOWN) || input_key_pressed(GLFW_KEY_DOWN);
}

void handlePlayerMovement(Player *player) {

  if (player->action[LEFT]) {
    player->velocity.x += -35.2f;
  }
  if (player->action[RIGHT]) {
    player->velocity.x += 35.2f;
  }

  if (player->action[UP] && player->onGround) {
    player->jump = true;
    player->onGround = false;
  }
  if (!player->action[LEFT] && !player->action[RIGHT]) {
    player->velocity.x *= .1f;
    if (fabs(player->velocity.x) < 5.01)
      player->velocity.x = 0.f;
  }

  player->velocity.x =
      clamp(player->velocity.x, -player->maxSpeedX, player->maxSpeedX);
}

#endif