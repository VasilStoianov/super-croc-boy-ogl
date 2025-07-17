#include "game.h"
#include "input.h"
#include "level/levels.h"
#include "level/tile.h"
#include "lib/glad.h"
#include "math/camera.h"
#include "math/physic.h"
#include "shaders/shader.h"
#include "stdbool.h"
#include "structs/animation.h"
#include "structs/player.h"
#include "vertices/vertices.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#define WIDTH 800
#define HEIGHT 600

void processInput(GLFWwindow *window);

float cameraX = 1.f;
bool shakeThecamera = false;
Player *player = {0};

  bool debug = false;

int main(void) {

  GLFWwindow *window = init(WIDTH, HEIGHT, "Super croc boy OPENGL");

  // create player
  player = createPlayer();

  load_player_animations(player);
  // create tile
  Level *lvl = load_leve1(text_shader.id);

  double time = glfwGetTime();
  double lastFrame = 0;
  double lastTime = time;
  int fps = 0;

  Camera *camera = create_camera();
  Circle *circle = create_circle(32, (vector){.x = 400.f, .y = 400.f},COLOR_RED);
  set_camera(camera);

  int frame = 0;
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (!glfwWindowShouldClose(window)) {
    camera->startShaking = shakeThecamera;
    time = glfwGetTime();
    fps++;

    if (time - lastTime >= .6)
      frame++;

    // fps update every 1 second
    update_time(&time, &lastTime, debug, &fps);
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int x = 0; x < lvl->background_count; x++) {

      draw_texture_matrix_id(lvl->layer1[x]->translate,
                             lvl->layer1[x]->texture.id);
    }

    for (int x = 0; x < lvl->background_count; x++) {

      draw_texture_matrix_id(lvl->layer2[x]->translate,
                             lvl->layer2[x]->texture.id);
    }

    for (int x = 0; x < lvl->background_count; x++) {

      draw_texture_matrix_id(lvl->background[x]->translate,
                             lvl->background[x]->texture.id);
    }

    // update player
    applyGravity(player, dt);
    updatePlayer(player, dt);

    player_ground_collision(player, lvl->tiles, lvl->tiles_count, dt);

    for (int x = 0; x < lvl->tiles_count; x++) {
      Tile *tile = lvl->tiles[x];
      draw_texture(&(tile->texture));
    }

    Animation *animation = &(player->animations[player->state]);
    Texture texture = animation->textures[animation->current_frame];
    draw_texture_matrix_id(player->translation, texture.id);
    handle_anim_frames(animation);

    move_camera(camera, player->size, player->position, lvl->size, dt);
update_angle(dt,lvl->rs);
    draw_circle_texture_rotation(lvl->rs->texture.translation,lvl->rs->texture.id,lvl->rs->rotation);
    draw_circle(circle);
     circle_rect_collision(circle->position,player->position,player->size,circle->radius);
    glfwSwapBuffers(window);
    glfwPollEvents();

    dt = time - lastFrame;
    if (dt < 0.01 || dt > 0.02) {
      dt = 1.f / 60.f;
    }
    lastFrame = time;
  }

  free(player);
  free(circle);
  free_leve(lvl);
  glfwTerminate();
  return 1;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    shakeThecamera = !shakeThecamera;
  }
  if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
    debug = !debug;
  }
  input_update();
  set_player_action(player);
  handlePlayerMovement(player);
}