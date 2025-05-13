#ifndef __game__
#define __game__

#include "math/camera.h"
#include "lib/glad.h"
#include "level/levels.h"
#include "level/tile.h"
#include "structs/player.h"
#include "utils.h"
#include "vertices/vertices.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>



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

void draw(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void move_camera(Player *player, Camera *camera, int shaderId, Level *level,
                 float deltaTime) {
  float camPlayerLine =
      camera->position.x + player->position.x - player->size.x / 2.f;

  if (camPlayerLine > 600.f &&
      (camera->position.x * -1.f) + 800.f < level->size.x) {
    camera->position.x -= 850.f * deltaTime;
  } else if (camPlayerLine < 200.f && camera->position.x * -1.f > 0) {
    camera->position.x += 850.f * deltaTime;
  }

  set_camera(camera, shaderId);
  shake_camera(camera, deltaTime, shaderId);
}

#endif
