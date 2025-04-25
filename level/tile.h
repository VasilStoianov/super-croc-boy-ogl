#ifndef TILE_H
#define TILE_H
#include "../math/matrix.h"
#include "../math/vector.h"
#include <stdlib.h>

typedef struct {
  vector position;
  mat4f translate;
  vector size;
} Tile;

Tile *createTile() {
  Tile *tile = (Tile *)malloc(sizeof(Tile));
  tile->translate = identity();
  tile->size = (vector){.x = .75f ,.y=1.f,.z=0.f};
  tile->position = (vector){.x = -.6f+tile->size.x/2.f, .y = -.75f+tile->size.y/2.f, .z = 0.f};
  return tile;
}

void create_tile_matrix(mat4f *tile_matrix, vector position) {
  *tile_matrix = identity();
  setTranslation(tile_matrix, position);
}


void draw_tile(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

#endif