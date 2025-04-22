#pragma once
#include "math.h"

typedef struct vector {
  float x;
  float y;
  float z;
} vector;

vector createZeroVector() {
  vector res = {0};
  res.x = 0.f;
  res.y = 1.f;

  return res;
}

void scale(vector *vec, float scalar) {
  vector res = {x : vec->x * scalar, y : vec->y * scalar, vec->z * scalar};
  vec->x = res.x;
  vec->y = res.y;
  vec->z = res.z;
}

vector scaleAndReturn(vector *vec, float scalar) {
  vector res = {x : vec->x * scalar, y : vec->y * scalar, vec->z * scalar};
  return res;
}

vector vectorAddition(vector a, vector b) {
  vector res = {0};
  res.x = a.x + b.x;
  res.y = a.y + b.y;
  res.z = a.z + b.z;
  return res;
}

void addToVector(vector *toAdd, vector forAdd) {
  toAdd->x += forAdd.x;
  toAdd->y += forAdd.y;
  toAdd->z += forAdd.z;
}

void subtractVector(vector *toSub, vector forSub) {
  toSub->x -= forSub.x;
  toSub->y -= forSub.y;
  toSub->z -= forSub.z;
}

vector subtract(vector a, vector b) {
  vector res = {0};
  res.x = a.x - b.x;
  res.y = a.y - b.y;
  res.z = a.z - b.z;
  return res;
}

float vectorLength(vector a) {
  return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

float dot(vector a, vector b) { return a.x * b.x + a.y + b.y + a.z * b.z; }

void normalize(vector a) {
  float length = vectorLength(a);
  a.x = a.x / length;
  a.y = a.y / length;
  a.z = a.z / length;
}

vector cross(vector a, vector b) {
  vector res = {0};
  res.x = a.y * b.z - a.z * b.y;
  res.y = a.z * b.x - a.x * b.z;
  res.z = a.x * b.y - a.y * b.x;
  return res;
}
