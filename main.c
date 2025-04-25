#include "game.h"
#include "glad.h"
#include "input.h"
#include "level/tile.h"
#include "player.h"
#include "shader.h"
#include "stdbool.h"
#include "vertices.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float dt = 1.0 / 60.f;
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

  // load vbo
  unsigned int VBO = 0, VAO = 0, EBO = 0;
  configVertices(&VBO, &VAO, &EBO);
  
  //create player
  player = createPlayer();
  
  //create tile
  Tile *tile = createTile();
  create_tile_matrix(&(tile->translate), tile->position);

  print_matrix(tile->translate);

  double time = glfwGetTime();
  double lastFrame = 0;
  double lastTime = time;
  int fps = 0;
  bool debug = false;
  // render loop
  while (!glfwWindowShouldClose(window)) {
    time = glfwGetTime();
    fps++;
//fps update every 1 second
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
    
    //update player 

    applyGravity(player, dt);
    player_groun_collision(player,tile);
    scalePlayer(player, (vector){.x = 0.25f, .y = 0.25f});
    updatePlayer(player, shader.id, dt);

    //draw player
    draw(VAO, shader.id);
    
    //draw tile
    set_matrix_uniform(tile->translate, shader.id);
    draw(VAO, shader.id);
    
    glfwSwapBuffers(window);
    glfwPollEvents();

    dt = time - lastFrame;
    if (dt < 0.01 || dt > 0.02) {
      dt = 1.f / 60.f;
    }
    lastFrame = time;
  }
  free(player);
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
  input_update();
  set_player_action(player);
  handlePlayerMovement(player);
}