#ifndef __player__
#define __player__
#include "glad.h"
#include "input.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "shader.h"
#include "utils.h"
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
  int action[LAST_DIR];
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

  return p;
}

void updatePlayer(Player *player, unsigned int prograId, float dt) {
  if (player->jump) {
    player->velocity.y -= 125.f;
    if (player->velocity.y < -player->maxSpeedY) {
      player->jump = false;
    }
  }

  player->position.x += player->velocity.x * dt;
  player->position.y += player->velocity.y * dt;

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