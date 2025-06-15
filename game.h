#ifndef __game__
#define __game__

#include "level/levels.h"
#include "level/tile.h"
#include "lib/glad.h"
#include "math/camera.h"
#include "structs/player.h"
#include "utils.h"
#include "vertices/vertices.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "lib/stb_image.h"

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

void config_texture_vertices(unsigned int *VBO, unsigned int *VAO,
                             unsigned int *EBO, char *image_path,
                             int shader_id,unsigned int *texture) {

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char *data = stbi_load(image_path, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
  }
  stbi_image_free(data);
  glUniform1i(glGetUniformLocation(shader_id, "texture1"), 0);
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void draw_square(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_texture(unsigned int texture,unsigned int VAO, unsigned int program_id) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
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
