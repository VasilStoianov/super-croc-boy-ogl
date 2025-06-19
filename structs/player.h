#ifndef __player__
#define __player__
#include "../input.h"
#include "../lib/glad.h"
#include "../math/matrix.h"
#include "../math/vector.h"
#include "../shaders/shader.h"
#include "../structs/aabb.h"
#include "../utils.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum Direction { LEFT, RIGHT, UP, DOWN, STOP, LAST_DIR } Direction;
typedef enum { RUN, FALL, JUMP, IDLE, LAST_ANIM } State;

typedef struct Player {

  vector position;
  vector velocity;
  vector size;

  vector scale;
  float maxSpeedX;
  float maxSpeedY;
  bool onGround;
  vector vertices[4];
  Direction action[LAST_DIR];
  Texture animations[LAST_ANIM][4];
  State state;
  mat4f translation;
  bool jump;
  AABB aabb;

} Player;

void load_animation(Player *player, int frames, State anim_type, char *path,
                    unsigned int shader_id) {

  if (frames > 1) {

    char *temp = malloc(strlen(path) + 7);
    strcpy(temp, path);
    strcat(temp, "-0.png");
    int len = strlen(temp);
    for (int x = 1; x < frames; x++) {
      temp[strlen(temp) - 5] = 48 + x;
      generate_texture(temp, &(player->animations[anim_type][x]), shader_id);
    }

  } else {

    Texture tex = player->animations[anim_type][0];
    tex.id = 0;
    generate_texture(path, &tex, shader_id);
  }
}

Player *createPlayer() {

  Player *p = (Player *)malloc(sizeof(Player));

  p->velocity = createZeroVector();
  p->position = createZeroVector();
  p->maxSpeedX = 500.f;
  p->maxSpeedY = 450.f;
  p->translation = identity();
  p->state = FALL;
  p->onGround = false;
  p->jump = false;
  p->size = (vector){.x = 60.f, .y = 75.f, .z = 0.f};
  p->position.x = 600.f;
  p->position.y = 65.f;

  for(int x = 0; x<LAST_ANIM;x++){
   for(int y = 0;y<4;y++){
     p->animations[x][y].id = 0;
     p->animations[x][y].width = 0;
     p->animations[x][y].heidth = 0;
   }
  }

  vector half_size = {.x = p->size.x / 2.f, .y = p->size.y / 2.f};
  vector min = {.x = p->position.x - half_size.x,
                .y = p->position.y - half_size.y};

  vector max = {.x = p->position.x + half_size.x,
                .y = p->position.y + half_size.y};

  p->scale = (vector){.x = p->size.x + 15, .y = p->size.y};

  p->vertices[0] = (vector){.x = p->position.x - p->size.x * 0.5f,
                            .y = p->position.y - p->size.y * 0.5f};
  p->vertices[1] = (vector){.x = p->position.x + p->size.x * 0.5f,
                            .y = p->position.y - p->size.y * 0.5f};
  p->vertices[2] = (vector){.x = p->position.x + p->size.x * 0.5f,
                            .y = p->position.y + p->size.y * 0.5f};
  p->vertices[3] = (vector){.x = p->position.x - p->size.x * 0.5f,
                            .y = p->position.y + p->size.y * 0.5f};

  set_aabb(&(p->aabb), min, max);
  return p;
}

void updatePlayer(Player *player, unsigned int prograId, float dt) {
  if (player->jump) {
    player->velocity.y -= 70.f;
    if (player->velocity.y < -player->maxSpeedY) {
      player->jump = false;
    }
  }

  player->position.x += player->velocity.x * dt;
  player->position.y += player->velocity.y * dt;
  vector half_size = {.x = player->size.x / 2.f, .y = player->size.y / 2.f};
  vector min = {.x = player->position.x - half_size.x,
                .y = player->position.y - half_size.y};

  vector max = {.x = player->position.x + half_size.x,
                .y = player->position.y + half_size.y};
  player->vertices[0] =
      (vector){.x = player->position.x - player->size.x * 0.5f,
               .y = player->position.y - player->size.y * 0.5f};
  player->vertices[1] =
      (vector){.x = player->position.x + player->size.x * 0.5f,
               .y = player->position.y - player->size.y * 0.5f};
  player->vertices[2] =
      (vector){.x = player->position.x + player->size.x * 0.5f,
               .y = player->position.y + player->size.y * 0.5f};
  player->vertices[3] =
      (vector){.x = player->position.x - player->size.x * 0.5f,
               .y = player->position.y + player->size.y * 0.5f};

  set_aabb(&(player->aabb), min, max);

  setTranslation(&(player->translation), player->position);
  set_matrix_uniform(player->translation, prograId, "translation");
}

void scalePlayer(Player *player) {

  scaleMatrix(&(player->translation), player->scale);
}
void set_player_action(Player *player) {

  player->action[LEFT] =
      input_key_held(GLFW_KEY_LEFT) || input_key_pressed(GLFW_KEY_LEFT);
  player->action[RIGHT] =
      input_key_held(GLFW_KEY_RIGHT) || input_key_pressed(GLFW_KEY_RIGHT);
  player->action[UP] =
      input_key_held(GLFW_KEY_UP) || input_key_pressed(GLFW_KEY_UP);
  player->action[DOWN] =
      input_key_held(GLFW_KEY_DOWN) || input_key_pressed(GLFW_KEY_DOWN);
}

void update_aabb_player(Player *player) {

  vector half_size = {.x = player->size.x / 2.f, .y = player->size.y / 2.f};
  vector min = {.x = player->position.x - half_size.x,
                .y = player->position.y - half_size.y};

  vector max = {.x = player->position.x + half_size.x,
                .y = player->position.y + half_size.y};

  set_aabb(&(player->aabb), min, max);
}

void handlePlayerMovement(Player *player) {

  if (player->action[LEFT]) {
    player->velocity.x += -50.2f;
    player->state = RUN;
    if (player->scale.x > 0.f) {
      player->scale.x *= -1.f;
    }
  }
  if (player->action[RIGHT]) {
    player->velocity.x += 50.2f;

    player->state = RUN;
    if (player->scale.x < 0.f) {
      player->scale.x *= -1.f;
    }
  }

  if (player->action[UP] && player->onGround) {
    player->jump = true;
    player->onGround = false;
  }
  if (!player->action[LEFT] && !player->action[RIGHT]) {
    player->velocity.x *= .4f;
    if (fabs(player->velocity.x) < 5.01)
      player->velocity.x = 0.f;
  }

  player->velocity.x =
      clamp(player->velocity.x, -player->maxSpeedX, player->maxSpeedX);
}

#endif