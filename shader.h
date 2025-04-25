#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "math/vector.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math/matrix.h"

typedef struct Shader {
  int id;
} Shader;

char *readFile(FILE *f) {

  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  rewind(f);

  char *result = malloc(length + 1); // +1 for null terminator
  if (!result) {
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  size_t readSize = fread(result, 1, length, f);
  result[readSize] = '\0';

  return result;
}

Shader createShader(char *filePath) {
  char *fullPathFs = malloc(strlen(filePath) + strlen("fragment.fs"));
  fullPathFs[0] = '\0';
  char *fullPathVs = malloc(strlen(filePath) + strlen("vertex.vs"));
  fullPathVs[0] = '\0';
  strcat(fullPathFs, filePath);
  strcat(fullPathFs, "fragment.fs");
  strcat(fullPathVs, filePath);
  strcat(fullPathVs, "vertex.vs");
  printf("[INFO] fragment shader file path %s\n", fullPathFs);
  printf("[INFO] vertex shader file path %s\n", fullPathVs);

  FILE *fragment = fopen(fullPathFs, "r");
  if (fragment == NULL) {
    printf("ERROR: Cannot open fragment file\n");
    exit(-1);
  }
  char *fragmentSource = readFile(fragment);

  fclose(fragment);
  free(fullPathFs);

  FILE *vertex = fopen(fullPathVs, "r");
  if (vertex == NULL) {
    printf("ERROR: Cannot open vertex file\n");
    exit(-1);
  }

  char *vertexSource = readFile(vertex);

  fclose(vertex);
  free(fullPathVs);

  unsigned int vertexId, fragmentId;
  int success;
  char infoLog[512];

  vertexId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexId, 1, (const char **)&vertexSource, NULL);
  glCompileShader(vertexId);
  glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
    printf("ERROR: %s\n", infoLog);
  }

  fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentId, 1, (const char **)&fragmentSource, NULL);
  glCompileShader(fragmentId);
  success = 0;
  glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);

    printf("ERROR: %s\n", infoLog);
  }

  Shader shader = {0};
  shader.id = glCreateProgram();
  glAttachShader(shader.id, vertexId);
  glAttachShader(shader.id, fragmentId);
  glLinkProgram(shader.id);

  glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader.id, 512, NULL, infoLog);
  }
  glDeleteShader(vertexId);
  glDeleteShader(fragmentId);
  free(fragmentSource);
  free(vertexSource);

  return shader;
}

void set_matrix_uniform(mat4f matrix, unsigned int shaderId) {
  GLint location = glGetUniformLocation(shaderId, "translation");
  glUseProgram(shaderId);
  glUniformMatrix4fv(location,1,GL_TRUE,(const float*)&matrix.m);

}

