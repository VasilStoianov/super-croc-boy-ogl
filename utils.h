#ifndef UTILS_H
#define UTILS_H
#include "math/vector.h"
#include <stdio.h>
#include "lib/glad.h"

#include "lib/stb_image.h"
#include <GLFW/glfw3.h>

#define COLOR_BLACK ((vector){.x = 0,.y = 0,.z=0})
#define COLOR_WHITE ((vector){255,255,255})
#define COLOR_RED   ((vector){255,0,0})
#define COLOR_LIME ((vector){0,255,0})
#define COLOR_BLUE ((vector){0,0,255})
#define COLOR_YELOOW ((vector){255,255,0}) 
#define COLOR_GRAY ((vector){128,128,128}) 

float dt = 1.0 / 60.f;

float clamp(float value, float min, float max) {
  if (value > max)
    return max;
  if (value < min)
    return min;
  return value;
}


void print_matrix(mat4f matrix){
    for(int x = 0; x<4; x++){
        for(int y = 0; y<4; y++){
            printf("%f ", matrix.m[x][y]);
        }
        printf("\n");
    }
}

void generate_texture(char* image_path, Texture* texture,unsigned int shader_id){
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
  int  nrChannels;
  
 stbi_set_flip_vertically_on_load(1);
  unsigned char *data = stbi_load(image_path, &(texture->width), &(texture->heidth), &nrChannels, 0);
  if (data) {
GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0,format,texture->width, texture->heidth, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("[ERROR] NO TEXTURE DATA\n");
  }
  stbi_image_free(data);
  glUseProgram(shader_id);
  glUniform1i(glGetUniformLocation(shader_id, "texture1"), 0);
}


#endif