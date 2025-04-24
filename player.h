#ifndef __player__
#define __player__
#include "glad.h"
#include "math/vector.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "input.h"

typedef enum Direction { LEFT, RIGHT, UP, DOWN, STOP, LAST_DIR } Direction;

typedef struct Player {

  vector position;
  vector velocity;
  float maxSpeedX;
  float maxSpeedY;
  int action[LAST_DIR];

} Player;

Player *createPlayer() {

  Player *p = (Player *)malloc(sizeof(Player));

  p->velocity = createZeroVector();
  p->position = createZeroVector();
  p->maxSpeedX = 1.f;
  p->maxSpeedY = 1.f;

  return p;
}

void updatePlayer(Player *player, unsigned int prograId, float dt) {

  player->position.x += player->velocity.x * dt;
  player->position.y += player->velocity.y * dt;
  setVecUniform(player->position, prograId);
}

void set_player_action(Player *player) {

  player->action[LEFT] =
      input_key_held(GLFW_KEY_LEFT) || input_key_pressed(GLFW_KEY_LEFT);
  player->action[RIGHT] =
      input_key_held(GLFW_KEY_RIGHT) || input_key_pressed(GLFW_KEY_RIGHT);
  player->action[UP] =
      input_key_held(GLFW_KEY_UP) || input_key_pressed(GLFW_KEY_UP);
  player->action[DOWN] =
      input_key_held(GLFW_KEY_DOWN) || input_key_pressed(GLFW_KEY_DOWN);
}

#endif