#include "game.h"
#include "input.h"
#include "level/levels.h"
#include "level/tile.h"
#include "lib/glad.h"
#include "math/camera.h"
#include "math/physic.h"
#include "shaders/shader.h"
#include "stdbool.h"
#include "structs/player.h"
#include "vertices/vertices.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float cameraX = 1.f;
bool shakeThecamera = false;
Player *player = {0};
int main(void) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Super croc boy OPENGL", NULL, NULL);

  if (window == NULL) {
    printf("No window\n");
    glfwTerminate();
    return -1;
  }

  input_init(window);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return -1;
  }

  // load shader
  char filePath[10] = "shaders/";
  Shader shader = createShader(filePath);
  char textFilePath[25] = "shaders/texture/";
  Shader text_shader = createShader(textFilePath);
  char *pic_path = "textures/run/frame-1.png";
  // load vbo
  unsigned int VBO = 0, VAO = 0, EBO = 0;

  unsigned int VBO_T = 0, VAO_T = 0, EBO_T = 0, texture;
  ;
  configVertices(&VBO, &VAO, &EBO);
  config_texture_vertices(&VBO_T, &VAO_T, &EBO_T, pic_path, text_shader.id,
                          &texture);

  // create player
  player = createPlayer();

  // create tile
  // Tile *tile = createTile();
  Level *lvl = load_leve1();

  double time = glfwGetTime();
  double lastFrame = 0;
  double lastTime = time;
  int fps = 0;
  bool debug = false;
  //   left,width,top, heidht, near,far)
  mat4f orthographic = ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f);
  // render loop
  Camera *camera = create_camera();
  set_camera(camera, shader.id);
  while (!glfwWindowShouldClose(window)) {
    camera->startShaking = shakeThecamera;
    time = glfwGetTime();
    fps++;
    // fps update every 1 second
    if (time - lastTime >= 1.0) {
      if (debug) {
        printf("FPS %d\n", fps);
      }
      fps = 0;
      lastTime = time;
    }

    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update player
    glUseProgram(shader.id);
    applyGravity(player, dt);
    updatePlayer(player, shader.id, dt);

    scalePlayer(player, (vector){.x = player->size.x, .y = player->size.y});

    // check_collision_gjk(player,lvl->tiles,lvl->tiles_count);
    player_ground_collision(player, lvl->tiles, lvl->tiles_count, dt);
    set_matrix_uniform(orthographic, shader.id, "projection");

    set_vec3_uniform(COLOR_RED, shader.id, "inColor");
    // draw player
    // draw_square(VAO, shader.id);
    // draw tile

    for (int x = 0; x < lvl->tiles_count; x++) {
      Tile *tile = lvl->tiles[x];

      set_matrix_uniform(tile->translate, shader.id, "translation");

      set_vec3_uniform(tile->color, shader.id, "inColor");
      draw_square(VAO, shader.id);
    }

    set_matrix_uniform(player->translation, text_shader.id, "translation");
    set_matrix_uniform(orthographic, text_shader.id, "projection");
    move_camera(player, camera, text_shader.id, lvl, dt);
    draw_texture(texture, VAO_T, text_shader.id);

    move_camera(player, camera, shader.id, lvl, dt);
    glfwSwapBuffers(window);
    glfwPollEvents();

    dt = time - lastFrame;
    if (dt < 0.01 || dt > 0.02) {
      dt = 1.f / 60.f;
    }
    lastFrame = time;
  }
  free(player);
  free_leve(lvl);
  glfwTerminate();
  return 1;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    shakeThecamera = !shakeThecamera;
  }
  input_update();
  set_player_action(player);
  handlePlayerMovement(player);
}