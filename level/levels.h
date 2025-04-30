#ifndef __LEVELS__
#define __LEVELS__
#include "tile.h"
#include <stdlib.h>
#include <stdio.h>
#define LEVEL_W 34
#define LEVEL_H 25
#define MAX_TILES (LEVEL_W * LEVEL_H)
//34x25
char* lvl1[] = 
{
// "########################",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                                          #",
"#                           #              #",
"#                         #      #         #",
"#                       #        #         #",
"############################################",
NULL};

typedef struct {

    Tile** tiles;
    int tiles_count;
} Level;

Level* load_leve1(){
    Level* level1 =(Level*) malloc(sizeof(Level));
    int counter = 0;
    int index = 0;
    int row = 0;
    level1->tiles_count = 0;
    Tile** tiles =(Tile**) malloc(350*(sizeof(Tile*)));
      while(lvl1[row] != NULL){   
        while(lvl1[row][index] != '\0' ){  
        if(lvl1[row][index] == '#'){
            Tile* tile =(Tile*) malloc(sizeof(Tile));
             vector size = {
                .x = 24.f,
                .y = 24.f
             };

            vector position = {
                .x =24.f*index,
                .y = 24.f*row 
            };
            tile = create_tile_with_pos_and_scale(position,size);
            tiles[counter++] = tile;
        }    
        index++;
        }
       index = 0;
       row++; 
    }   
    level1->tiles_count = counter;
    level1->tiles = tiles;
    return level1; 
}



void free_leve(Level* level){
    for(int x = 0; x<level->tiles_count;x++){
        free(level->tiles[x]);
    }
    free(level);
}




















#endif