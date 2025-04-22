#include "game.h"
#include "glad.h"
#include "player.h"
#include "shader.h"
#include "stdbool.h"
#include "vertices.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float dt = 1.0 / 60.f;
bool keys[1024];
Player *player;

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
  unsigned int VBO, VAO, EBO;
  configVertices(VBO, VAO, EBO);

  player = createPlayer();

  glfwSetKeyCallback(window, key_callback);

  double time = glfwGetTime();
  double lastFrame = time;
  printf("%f %f \n",dt,lastFrame);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    time = glfwGetTime();
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updatePlayer(player, shader.id, dt);
    glUseProgram(shader.id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    dt = time - lastFrame;
    if(dt<0.01){
        dt = 1.f/60.f;
    } 
       printf("dt : %f\n",dt);
    lastFrame = time;
  }

  glfwTerminate();
  return 1;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      keys[key] = true;
    } else if (action == GLFW_RELEASE) {
      keys[key] = false;
    }
  }
}

void processInput(GLFWwindow *window) {

  if (keys[GLFW_KEY_ESCAPE]) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (keys[GLFW_KEY_RIGHT]) {
    updatePlayerVelocity(player, 1.f, RIGHT, dt);
    return;
  }

  if (keys[GLFW_KEY_LEFT]) {
    updatePlayerVelocity(player, -1.f, LEFT, dt);
    return;
  }
  updatePlayerVelocity(player, 0.f, STOP, dt);
}