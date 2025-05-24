#ifndef TILE_H
#define TILE_H
#include "../math/matrix.h"
#include "../math/vector.h"
#include <stdlib.h>

typedef struct {
  vector position;
  mat4f translate;
  vector size;
  float scale;
  vector vertices[4];
} Tile;

Tile *createTile() {
  Tile *tile = (Tile *)malloc(sizeof(Tile));
  tile->translate = identity();
  tile->size = (vector){.x = .75f*.5f ,.y=1.f*.5f,.z=0.f};
  tile->position = (vector){.x = -.6f+tile->size.x/2.f, .y = -1.255f+tile->size.y/2.f, .z = 0.f};
  return tile;
}



void set_tile_translation(mat4f *tile_matrix, vector position) {
  setTranslation(tile_matrix, position);
}

Tile *create_tile_with_pos_and_scale(vector position,vector size) {
  Tile *tile = (Tile *)malloc(sizeof(Tile));
  tile->translate = identity();
  tile->size = size; 
  tile->position =  position;
  tile->vertices[0] = (vector){.x = position.x - size.x,.y = position.y -size.y };
  tile->vertices[1] = (vector){.x = position.x + size.x,.y = position.y -size.y };
  tile->vertices[2] = (vector){.x = position.x + size.x,.y = position.y +size.y };
  tile->vertices[3] = (vector){.x = position.x - size.x,.y = position.y +size.y };
  
  set_tile_translation(&(tile->translate),tile->position);
  scaleMatrix(&(tile->translate),size);
   return tile;
}

void draw_tile(unsigned int VAO, unsigned int program_id) {
  glUseProgram(program_id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

#endif