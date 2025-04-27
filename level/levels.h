#ifndef __LEVELS__
#define __LEFELS__
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
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#                               #",
"#################################"};

typedef struct {

    Tile** tiles;
    int tiles_count;
} Level;

Level* load_leve1(){
    Level* level1 =(Level*) malloc(sizeof(Level));
    int counter = 0;
    level1->tiles_count = 0;
    Tile** tiles =(Tile**) malloc(111*(sizeof(Tile*)));
    for(int x = 0; x<25; x++){
        for(int y = 0; y<34; y++){
         if(lvl1[x][y] == '#'){
            Tile* tile =(Tile*) malloc(sizeof(Tile));
             vector size = {
                .x = 24.f,
                .y = 24.f
             };

            vector position = {
                .x =24.f*y,
                .y = 24.f*x 
            };
            tile = create_tile_with_pos_and_scale(position,size);
            printf("%d \n",counter);
            tiles[counter++] = tile;
        }    
        
        }
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