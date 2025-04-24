#ifndef __game__
#define __game__

#include "glad.h"
#include "player.h"
#include "utils.h"
#include "vertices.h"
#include <GLFW/glfw3.h>

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

void drawPlayer(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

#endif
