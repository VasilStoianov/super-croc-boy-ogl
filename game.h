#ifndef __game__
#define __game__

#include "glad.h"
#include "level/tile.h"
#include "player.h"
#include "utils.h"
#include "vertices.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

static const float GRAVITY = -125.f;

void applyGravity(Player *player, float dt) {

  if (!player->onGround) {
    player->velocity.y += GRAVITY * dt;
  }
}

void configVertices(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO) {

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void handlePlayerMovement(Player *player) {

  if (player->action[LEFT]) {
    // updatePlayerVelocity(player, -.2f, 0.f);
    player->velocity.x += -.2f;
  }
  if (player->action[RIGHT]) {
    // updatePlayerVelocity(player,.2f, 0.f);
    player->velocity.x += .2f;
  }

  if (player->action[UP]) {
    // updatePlayerVelocity(player, 0.f, .2f);
    player->velocity.y += .2f;
  }
  if (player->action[DOWN]) {
    // updatePlayerVelocity(player,0.f,-.2f);
    player->velocity.y += -.2f;
  }
  if (!player->action[LEFT] && !player->action[RIGHT]) {
    //  updatePlayerVelocity(player, 0.f, 0.f);
    player->velocity.x = 0.f;
  }

  if (!player->action[UP] && !player->action[DOWN]) {
    player->velocity.y = 0.f;
  }

  player->velocity.x =
      clamp(player->velocity.x, -player->maxSpeedY, player->maxSpeedX);
  player->velocity.y =
      clamp(player->velocity.y, -player->maxSpeedY, player->maxSpeedX);
}

void draw(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool collision_check(Player *p, Tile *tile) {

  float halfWidth = p->size.x / 2.0f;
  float halfHeight = p->size.y / 2.0f;

  float playerLeft = p->position.x - halfWidth;
  float playerRight = p->position.x + halfWidth;
  float playerTop = p->position.y + halfHeight;
  float playerBottom = p->position.y - halfHeight;

  float halfWidthTile = tile->size.x / 2.0f;
  float halfHeightTile = tile->size.y / 2.0f;

  float tileLeft = tile->position.x - halfWidthTile;
  float tileRight = tile->position.x + halfWidthTile;
  float tileTop = tile->position.y + halfHeightTile;
  float tileBottom = tile->position.y - halfHeightTile;

  return playerRight > tileLeft && playerLeft < tileRight &&
         playerTop >= tileBottom && playerBottom <= tileTop;
}

void player_groun_collision(Player *p, Tile *tile) {

  if (collision_check(p, tile)) {
    if (p->velocity.y > 0) {
      p->position.y = tile->position.y - p->size.y;
    } else if (p->velocity.y < 0) {
      p->position.y = p->size.y / 2.f + tile->position.y + tile->size.y / 2.f;
      p->velocity.y = 0;
      p->onGround = true;
    }
  } else {

    p->onGround = false;
  }
}

#endif
