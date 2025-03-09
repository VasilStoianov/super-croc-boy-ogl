#include "SFML/Graphics.h"
#include "SFML/Window.h"
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "SFML/System.h"

void spriteDestroy(Player * player);



void spriteDestroy(Player * player){

  for(int x = 0; x<3; x++){
    int frames = player->frames[x];
    for(int y = 0; y<frames; y++){
      free(player->sprites[x][y]);
    }
    free(player->sprites[x]);
  }   

}


int main() {
 
  sfRectangleShape *rect = sfRectangleShape_create();
  sfRectangleShape_setSize(rect, (sfVector2f){300.f, 300.f});
  sfRectangleShape_setPosition(rect, (sfVector2f){150.0f, 200.0f});
  sfRectangleShape_setFillColor(rect, sfRed);
  sfRenderStates state = sfRenderStates_default();
  sfEvent event;
  Player *player = malloc(sizeof(Player));
  player->animationLoaded = 1;
  char * idle = "./textures/idle/frame-";
  char * dizzy = "./textures/dizzy/frame-";
  char * path = "./textures/run/frame-";
  loadSprite(&player,idle,2);
  loadSprite(&player,dizzy,2);
  loadSprite(&player,path,4);
  int index = hash(idle)%3;
  int animFrames = player->frames[index];
  size_t frame = 0;
  
 sfClock * time = sfClock_create();
 sfTime startTime = sfClock_restart(time);
 sfTime elapsedTime = sfClock_getElapsedTime(time);
 float timeAsSeconds = sfTime_asSeconds(elapsedTime); 
 
 sfVideoMode mode;
  mode.height = 600;
  mode.width = 600;
  mode.bitsPerPixel = 32;
  sfRenderWindow *renderer =
      sfRenderWindow_create(mode, "test", sfResize | sfClose, NULL);

  if (!renderer)
    return EXIT_FAILURE;

  sfRenderWindow_setFramerateLimit(renderer,60);
   while (sfRenderWindow_isOpen(renderer)) {
    elapsedTime = sfClock_getElapsedTime(time);
    timeAsSeconds = sfTime_asMilliseconds(elapsedTime);
   
    sfRenderWindow_clear(renderer, sfBlack);
    while (sfRenderWindow_pollEvent(renderer, &event)) {
      if (event.key.type == sfEvtClosed) {
        sfRenderWindow_close(renderer);
      }
      if(event.key.code == sfKeyEscape){
        sfRenderWindow_close(renderer);
      }
    }
    sfRenderWindow_drawRectangleShape(renderer,rect,&state);
    sfRenderWindow_drawSprite(renderer,player->sprites[index][frame],&state);
   
    if(timeAsSeconds > 155){
          frame++;
          if(frame >= animFrames) frame = 0;
         sfClock_restart(time);
        timeAsSeconds = sfTime_asSeconds(elapsedTime);
        } 
  
    sfRenderWindow_display(renderer);
  }

  sfClock_destroy(time);
  spriteDestroy(player);

  return 1;
}