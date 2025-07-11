#pragma once
#define __TEXTURE__
#include "../math/vector.h"
#include "../math/matrix.h"
typedef struct {
int width;
int heidth; 
unsigned int id;
vector position;
mat4f translation;
} Texture;


