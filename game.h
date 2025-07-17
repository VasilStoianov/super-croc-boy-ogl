#ifndef __game__
#define __game__

#include "lib/glad.h"

#include <GLFW/glfw3.h>
#include "input.h"
#include "lib/stb_image.h"
#include "shaders/shader.h"
#include "structs/circle.h"
#include "structs/square.h"
#include "structs/texture.h"
#include "utils.h"
#include "vertices/vertices.h"
#include <stdbool.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

unsigned int VBO = 0, VAO = 0, EBO = 0;

unsigned int VBO_T = 0, VAO_T = 0, EBO_T = 0;

unsigned int VBO_C = 0, VAO_C = 0;

unsigned int VBO_C_T, VAO_C_T;

Shader rect_shader;
Shader text_shader;
Shader circle_shader;
Shader circle_shader_t;
int circle_verts;
int circle_text_verts;

mat4f orthographic;
mat4f camera;

void configVertices(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO) {

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void config_texture_vertices(unsigned int *VBO, unsigned int *VAO,
                             unsigned int *EBO) {

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texture_vertices), texture_vertices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // safely unbind
}

void config_circle_vertices(unsigned int *VBO, unsigned int *VAO,
                            float *vertices, int circle_count) {

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * circle_count, vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // safely unbind
}

void config_circle_texture_vertices(unsigned int *VBO, unsigned int *VAO,
                                    float *vertices, int circle_count) {

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * circle_count, vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // safely unbind
}

void set_camera_matrix(mat4f matrix, vector position) {
  setTranslation(&matrix, position);
  set_matrix_uniform(matrix, rect_shader.id, "camera");
  set_matrix_uniform(matrix, text_shader.id, "camera");
  set_matrix_uniform(matrix, circle_shader.id, "camera");
}

void set_camera_shake(mat4f shake_matrix, vector shake) {

  setTranslation(&shake_matrix, shake);
  set_matrix_uniform(shake_matrix, rect_shader.id, "shake");
  set_matrix_uniform(shake_matrix, text_shader.id, "shake");
}

void set_matrices_uniform(mat4f translation, unsigned int shader_id) {
  set_matrix_uniform(translation, shader_id, "translation");
  set_matrix_uniform(orthographic, shader_id, "projection");
  set_matrix_uniform(camera, shader_id, "camera");
}

void set_matrices_uniform_rotation(mat4f translation,mat4f rotation, unsigned int shader_id) {
  set_matrix_uniform(translation, shader_id, "translation");
  set_matrix_uniform(orthographic, shader_id, "projection");
  set_matrix_uniform(camera, shader_id, "camera");
  set_matrix_uniform(rotation,shader_id,"rotation");
}

void draw_square(Square *square) {
  glUseProgram(rect_shader.id);
  set_matrices_uniform(square->translation, rect_shader.id);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_circle_texture(mat4f translate, unsigned int texture) {

  // Bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUseProgram(circle_shader_t.id);
  glBindVertexArray(VAO_C_T);
  set_matrices_uniform(translate, circle_shader_t.id);

  glDrawArrays(GL_TRIANGLE_FAN, 0, circle_text_verts);
}

void draw_circle_texture_rotation(mat4f translate,unsigned int texture, mat4f rotation ) {

  // Bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUseProgram(circle_shader_t.id);
  glBindVertexArray(VAO_C_T);
  set_matrices_uniform_rotation(translate, rotation,circle_shader_t.id);

  glDrawArrays(GL_TRIANGLE_FAN, 0, circle_text_verts);
}

void draw_circle(Circle *circle) {
  glUseProgram(circle_shader.id);
  set_matrices_uniform(circle->translation, circle_shader.id);
  set_vec3_uniform(circle->color, circle_shader.id, "inColor");
  glBindVertexArray(VAO_C);
  glDrawArrays(GL_TRIANGLE_FAN, 0, circle_verts);
}

void draw_texture_matrix_id(mat4f translation, unsigned int text_id) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, text_id);

  glUseProgram(text_shader.id);
  glBindVertexArray(VAO_T);
  set_matrices_uniform(translation, text_shader.id);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_texture(Texture *text) {
  draw_texture_matrix_id(text->translation, text->id);
}

void generate_texture(char *image_path, Texture *texture) {
  texture->id = 0;

  glGenTextures(1, &(texture->id));
  glBindTexture(GL_TEXTURE_2D, texture->id);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int nrChannels;

  stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load(image_path, &(texture->width),
                                  &(texture->heidth), &nrChannels, 0);
  if (data) {
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->heidth, 0,
                 format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("[ERROR] NO TEXTURE DATA\n");
  }
  stbi_image_free(data);
  glUseProgram(text_shader.id);
  glUniform1i(glGetUniformLocation(text_shader.id, "texture1"), 0);
}

void generate_texture_circle(char *image_path, Texture *texture) {
  texture->id = 0;

  glGenTextures(1, &(texture->id));
  glBindTexture(GL_TEXTURE_2D, texture->id);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int nrChannels;

  stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load(image_path, &(texture->width),
                                  &(texture->heidth), &nrChannels, 0);
  if (data) {
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->heidth, 0,
                 format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("[ERROR] NO TEXTURE DATA\n");
  }
  stbi_image_free(data);
  glUseProgram(circle_shader_t.id);
  glUniform1i(glGetUniformLocation(circle_shader_t.id, "texture1"), 0);
}

void update_time(double *time, double *lastTime, bool debug, int *fps) {
  if (time - lastTime >= 1.0) {
    if (debug) {
      printf("FPS %f\n", *fps);
    }
    fps = 0;
    lastTime = time;
  }
}
void update_cam(mat4f cam) { camera = cam; }

GLFWwindow *init(int width, int heidth, char *win_name) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow *window = glfwCreateWindow(width, heidth, win_name, NULL, NULL);

  if (window == NULL) {
    printf("No window\n");
    glfwTerminate();
    exit(-1);
  }

  input_init(window);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    exit(-1);
  }

  // load shader

  char filePath[10] = "shaders/";
  rect_shader = createShader(filePath);
  char textFilePath[25] = "shaders/texture/";
  text_shader = createShader(textFilePath);
  char circle_Path[25] = "shaders/circle/";
  circle_shader = createShader(circle_Path);
  char circle_text[35] = "shaders/circle-texture/";
  circle_shader_t = createShader(circle_text);

  printf("[INFO] shaders loaded\n");
  // load vbo
  // load square vertices
  configVertices(&VBO, &VAO, &EBO);

  // load texture vertices
  config_texture_vertices(&VBO_T, &VAO_T, &EBO_T);

  int out_count_c_v = 0;
  float *circle_t_vert = circle_vertices_texture(1.f, 64, &out_count_c_v);

  config_circle_texture_vertices(&VBO_C_T, &VAO_C_T, circle_t_vert,
                                 out_count_c_v);

  int out_count = 0;
  float *vert = circle_vertices(0.f, 0.f, 1.f, 32, &out_count);

  config_circle_vertices(&VBO_C, &VAO_C, vert, out_count);
  circle_verts = out_count;
  circle_text_verts = out_count_c_v;
  orthographic = ortho(0.f, (float)width, 0.f, (float)heidth, -1.f, 1.f);
  camera = identity();
  return window;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
#endif
