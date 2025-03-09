#pragma once
#include "SFML/Graphics.h"
#include "SFML/System.h"
#include <string.h>

typedef struct Player {
  sfVector2f position;
  sfVector2f velocity;
  sfSprite ***sprites;
  int frames[3];
  int animationLoaded;
} Player;

int hash(char *string) {

  int num = 0;
  int len = strlen(string);
  for (int x = 0; x < len; x++) {

    num += string[x] + x;
  }

  return num + len;
}

void loadSprite(Player **player, char *path, int frames) {
    int animations = 3;
    if((*player)->animationLoaded){
    (*player)->sprites = (sfSprite***) malloc(animations * sizeof(sfSprite ***));
    (*player)->animationLoaded = 0;
    }
    if (!(*player)->sprites) {
      printf("ERROR: Cannot allocate sprites\n");
    }
  

  int index = hash(path) % animations;
  (*player)->frames[index] = frames;
  (*player)->sprites[index] = malloc(frames * sizeof(sfSprite**));
  
  for (int x = 1; x <= frames; x++) {
    char *tempPath;
    int length = strlen(path) + snprintf(NULL, 0, "%d", x) + 5;
    tempPath = (char *)malloc(length);
    strcpy(tempPath, path);
    snprintf(tempPath + strlen(path), length - strlen(tempPath), "%d.png", x);
    printf("%s\n", tempPath);

    sfTexture *text = sfTexture_createFromFile(
        tempPath,
        &(sfIntRect){
            .top = 0.f, .left = 25.f, .width = 290.f, .height = 409.f});
     (*player)->sprites[index][x - 1]  = sfSprite_create();
    sfSprite_setTexture ((*player)->sprites[index][x - 1], text, 1);
    sfSprite_setPosition((*player)->sprites[index][x - 1], (sfVector2f){125.f, 125.f});
    sfSprite_setScale   ((*player)->sprites[index][x - 1], (sfVector2f){.x = 0.5, .y = 0.5});
    // sfTexture_destroy(text);
    free(tempPath);
  }
}