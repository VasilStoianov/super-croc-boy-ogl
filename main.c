#include "game.h"
#include "glad.h"
#include "input.h"
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

  player = createPlayer();

  // glfwSetKeyCallback(window, key_callback);

  double time = 0;
  double lastFrame = 0;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    time = glfwGetTime();
    input_update();
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader.id);

    updatePlayer(player, shader.id, dt);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    dt = time - lastFrame;
    if (dt < 0.01 || dt > 0.02) {
      dt = 1.f / 60.f;
    }
    printf("dt : %f\n", dt);
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
  set_player_action(player);

  if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window,1);
  }

  if (player->action[LEFT]) {
    updatePlayerVelocity(player, -.2f, 0.f);
  }
  if(player->action[RIGHT]){
    updatePlayerVelocity(player,.2f, 0.f);
  }

  if(player->action[UP]){
    updatePlayerVelocity(player, 0.f, .2f);
  }
if(player->action[DOWN]){
    updatePlayerVelocity(player,0.f,-.2f);
  }
  if (!player->action[LEFT] && !player->action[RIGHT] && !player->action[UP] && !player->action[DOWN]) {
    updatePlayerVelocity(player, 0.f, 0.f);
  }
  
}