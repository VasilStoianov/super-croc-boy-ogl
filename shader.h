#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "glad.h"
#include "GLFW/glfw3.h"

typedef struct Shader{
 int id;
} Shader;


Shader createShader(char *filePath){
    char* fileWi = malloc(strlen(filePath));
   strcpy(fileWi,filePath);
   strcat(fileWi,"vertex.vs");
   
   
   FILE *file = fopen(fileWi,"r");
   if(file == NULL){
    printf("ERROR: Cannot open shader FILE!\n");
   }

    fseek(file,0,SEEK_END);
    int end = ftell(file);
    rewind(file);

    char  *source = (char*)malloc(end+1);
    if(source == NULL)printf("ERROR: Cannot allocate memory for shader source!\n");


    fread(source,1,end,file);
    
    source[end] = '\0';
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,(const char**)&source,NULL);
    glCompileShader(vertexShader);
  int success;
  char infoLog[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("%s \n", infoLog);
  }
    fclose(file);
    printf("INF: File closed!\n");
    memset(&fileWi,0,sizeof(fileWi));
    fileWi = strcat(filePath,"fragment.fs");
    
    file = fopen(fileWi,"r");
     
     if(file ==NULL) printf("ERROR: Cannot open file!\n");
    fseek(file,0,SEEK_END);

    end = ftell(file);
    rewind(file);
    source = (char*)malloc(end+1);

    fread(source,1,end,file);

    source[end] = '\0';
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,(const char**)&source,NULL);
    glCompileShader(fragmentShader);
glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("%s \n", infoLog);
  }
    Shader shader = {0};
    shader.id = glCreateProgram();
    glAttachShader(shader.id,vertexShader);
    glAttachShader(shader.id,fragmentShader);
    glLinkProgram(shader.id);

    
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
    free(source);
    fclose(file);
  return shader;
}
