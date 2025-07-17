#ifndef __LEVELS__
#define __LEVELS__
#include "tile.h"
#include "../game.h"
#include "round_tile.h"
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
    "1                               ####                                        1",
    "1                   ####                     ####                           1",
    "1                             0             ##  ##                          1",
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
  Tile *background[7];
  Round_Spike* rs;
  Tile *layer1[7];
  Tile *layer2[7];
  short background_count;
} Level;

Level *load_leve1() {
  Level *level1 = (Level *)malloc(sizeof(Level));
  int counter = 0;
  int index = 0;
  int row = 0;
  int line = 0;
  level1->background_count = 7;
  level1->tiles_count = 0;
  Tile **tiles = (Tile **)malloc(350 * (sizeof(Tile *)));
  Texture wood;
  Texture grass;
  Texture chainsaw;
  char path[25] = "textures/grass.png";
  char pat2[25] = "textures/wood.png";
  char path3[25] = "textures/chainsaw.png";
  generate_texture(path, &grass);
  generate_texture(pat2, &wood);
  generate_texture_circle(path3,&chainsaw);
  while (lvl1[row] != NULL) {
    while (lvl1[row][index] != '\0') {

      switch (lvl1[row][index])
      {
      case '#':
        {
        Tile *tile = (Tile *)malloc(sizeof(Tile));
        vector size = {.x = 48.f, .y = 48.f};
        vector position = {.x = 48.f * index, .y = 48.f * row};

        tile = create_tile_with_pos_and_scale(position, size);
        tile->texture = grass;
        tile->texture.translation = tile->translate;
        tiles[counter++] = tile;
        break;
      }
       
      case '1':{
        Tile *tile = (Tile *)malloc(sizeof(Tile));
        vector size = {.x = 48.f, .y = 48.f};
        vector position = {.x = 48.f * index, .y = 48.f * row};

        tile = create_tile_with_pos_and_scale(position, size);
        tile->texture = wood;

        tile->texture.translation = tile->translate;
        tiles[counter++] = tile;
        break;
      }
      case '0':{
       level1->rs = create_spike(24,(vector){.x= 48.f*index,.y = 48.f*row});
       level1->rs->texture = chainsaw;
       level1->rs->texture.translation = identity();
       setTranslation(&(level1->rs->texture.translation),level1->rs->position);
       scaleMatrix(&(level1->rs->texture.translation),(vector){.x = 48.f,.y = 48.f});

        break;
      }

      default:
        break;
      }
        


      index++;
    }
    line = index;
    index = 0;
    row++;
  }

 Texture background;
  generate_texture("textures/back/3.png", &background);
  Texture lay1;
  generate_texture("textures/back/1.png", &lay1);
  Texture lay2;
  generate_texture("textures/back/2.png", &lay2);
 
  for(int c = 0; c<level1->background_count;c++){
    
   level1->background[c] = create_tile_with_pos_and_scale((vector){.x=(c*background.width),.y = 385.f},
  (vector){.x = background.width,.y = background.heidth});
  level1->background[c]->texture = background;  
 
  level1->layer1[c] = create_tile_with_pos_and_scale((vector){.x=(c*lay1.width),.y = 170.f},
  (vector){.x = lay1.width,.y = lay1.heidth});
  level1->layer1[c]->texture = lay1;  

  level1->layer2[c] = create_tile_with_pos_and_scale((vector){.x=(c*lay2.width),.y = 300.f},
  (vector){.x = lay2.width,.y = lay2.heidth});
  level1->layer2[c]->texture = lay2;  

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