#ifndef __player__
#define __player__
#include "glad.h"
#include <GLFW/glfw3.h>
#include "math/vector.h"
#include <stdlib.h>
#include "shader.h"

typedef struct Player {

  vector position;
  vector velocity;

} Player;

typedef enum Direction { LEFT, RIGHT, UP, DOWN, STOP } Direction;
void updatePlayerVelocity(Player *player, float updateValue, Direction dir,float dt) {
  switch (dir) {
  case RIGHT: {
    player->velocity.x += updateValue;
    printf("%f %f\n",dt,player->velocity.x);
    break;
  }
  case LEFT: {

    printf("%f %f\n",dt,player->velocity.x);
    player->velocity.x += updateValue;
    break;
  }
  case STOP: {
    player->velocity.y = 0.f;
    player->velocity.x = 0.f;
    break;
  }

  default:
    break;
  }
}
Player *createPlayer() {

  Player *p = (Player *)malloc(sizeof(Player));

  p->velocity = createZeroVector();
  p->position = createZeroVector();

  return p;
}

void updatePlayer(Player *player, unsigned int prograId, float dt) {

  player->position.x += player->velocity.x * dt;
  player->position.y += player->velocity.y * dt;
  setVecUniform(player->position, prograId);
}

#endif