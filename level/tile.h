#ifndef TILE_H
#define TILE_H
#include "../math/matrix.h"
#include "../math/vector.h"
#include "../structs/aabb.h"
#include "../utils.h"

#include "../lib/glad.h"
#include <stdlib.h>

#include <GLFW/glfw3.h>
typedef struct {
  vector position;
  mat4f translate;
  vector size;
  float scale;
  vector vertices[4];
  AABB aabb;
  vector color;
} Tile;

Tile *createTile() {
  Tile *tile = (Tile *)malloc(sizeof(Tile));
  tile->translate = identity();
  tile->size = (vector){.x = .75f * .5f, .y = 1.f * .5f, .z = 0.f};
  tile->position = (vector){.x = -.6f + tile->size.x / 2.f,
                            .y = -1.255f + tile->size.y / 2.f,
                            .z = 0.f};
  return tile;
}

void set_tile_translation(mat4f *tile_matrix, vector position) {
  setTranslation(tile_matrix, position);
}

Tile *create_tile_with_pos_and_scale(vector position, vector size) {
  Tile *tile = (Tile *)malloc(sizeof(Tile));
  tile->translate = identity();
  tile->size = size;
  tile->position = position;

  vector half_size = {.x = tile->size.x / 2.f, .y = tile->size.y / 2.f};
  vector min = {.x = tile->position.x - half_size.x,
                .y = tile->position.y - half_size.y};

  vector max = {.x = tile->position.x + half_size.x,
                .y = tile->position.y + half_size.y};

  tile->vertices[0] =
      (vector){.x = position.x - size.x*0.5f, .y = position.y - size.y*0.5f};
  tile->vertices[1] =
      (vector){.x = position.x + size.x*0.5f, .y = position.y - size.y*0.5f};
  tile->vertices[2] =
      (vector){.x = position.x + size.x*0.5f, .y = position.y + size.y*0.5f};
  tile->vertices[3] =
      (vector){.x = position.x - size.x*0.5f, .y = position.y +  size.y*0.5f};

  set_aabb(&(tile->aabb), min, max);
  tile->color = COLOR_BLACK;

  set_tile_translation(&(tile->translate), tile->position);
  scaleMatrix(&(tile->translate), size);
  return tile;
}

void update_aabb_tile(Tile *tile) {
  vector half_size = {.x = tile->size.x / 2.f, .y = tile->size.y / 2.f};
  vector min = {.x = tile->position.x - half_size.x,
                .y = tile->position.y - half_size.y};

  vector max = {.x = tile->position.x + half_size.x,
                .y = tile->position.y + half_size.y};

  set_aabb(&(tile->aabb), min, max);
}

void draw_tile(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

#endif