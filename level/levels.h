#ifndef __LEVELS__
#define __LEVELS__
#include "tile.h"

#include <stdio.h>
#include <stdlib.h>
#define LEVEL_W 34
#define LEVEL_H 25
#define MAX_TILES (LEVEL_W * LEVEL_H)
// 34x25
char *lvl1[] = {
    // "########################",
    "1                                                                           1",
    "1                                                                           1",
    "1                                                                           1",
    "1                                                                           1",
    "1                            ##                                             1",
    "1                     ##                                                    1",
    "1                                                                           1",
    "1                                                                           1",
    "1                                                                           1",
    "1                                       1                    1              1",
    "1                             1                            1      1         1",
    "1                      1                                 1        1         1",
    "1###########################################################################1",
    NULL};
typedef struct {

  Tile **tiles;
  int tiles_count;
  vector size;
} Level;

Level *load_leve1(unsigned int shader_id) {
  Level *level1 = (Level *)malloc(sizeof(Level));
  int counter = 0;
  int index = 0;
  int row = 0;
  int line = 0;
  level1->tiles_count = 0;
  Tile **tiles = (Tile **)malloc(350 * (sizeof(Tile *)));
  Texture wood;
  Texture grass;
  char path[25] = "textures/grass.png";
  char pat2[25] = "textures/wood.png";
  generate_texture(path, &grass, shader_id);
  generate_texture(pat2, &wood, shader_id);
  while (lvl1[row] != NULL) {
    while (lvl1[row][index] != '\0') {
      if (lvl1[row][index] == '#') {
        Tile *tile = (Tile *)malloc(sizeof(Tile));
        vector size = {.x = 48.f, .y = 48.f};
        vector position = {.x = 48.f * index, .y = 48.f * row};

        tile = create_tile_with_pos_and_scale(position, size, path, shader_id);
        tile->texture = grass;
        tiles[counter++] = tile;
      }

      if (lvl1[row][index] == '1') {
        Tile *tile = (Tile *)malloc(sizeof(Tile));
        vector size = {.x = 48.f, .y = 48.f};
        vector position = {.x = 48.f * index, .y = 48.f * row};

        tile = create_tile_with_pos_and_scale(position, size,
                                              "textures/wood.png", shader_id);
        tile->texture = wood;
        tiles[counter++] = tile;
      }

      index++;
    }
    line = index;
    index = 0;
    row++;
  }

  level1->size = (vector){.x = line * 48.f, .y = row * 48.f};

  level1->tiles_count = counter;
  level1->tiles = tiles;
  return level1;
}

void free_leve(Level *level) {
  for (int x = 0; x < level->tiles_count; x++) {
    free(level->tiles[x]);
  }
  free(level);
}

#endif