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
  res.y = 0.f;

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

void normalize(vector *v) {
float len = sqrtf(v->x * v->x + v->y * v->y);
    if (len > 1e-6f) {
        v->x /= len;
        v->y /= len;
    } else {
        v->x = 0.f;
        v->y = 0.f;
    }
}

vector cross(vector a, vector b) {
  vector res = {0};
  res.x = a.y * b.z - a.z * b.y;
  res.y = a.z * b.x - a.x * b.z;
  res.z = a.x * b.y - a.y * b.x;
  return res;
}

vector tripleCross2D(vector a, vector b) {
    float dot_a_b = a.x * b.x + a.y * b.y;
    float dot_a_a = a.x * a.x + a.y * a.y;

    // result = b - a * (dot(b, a) / dot(a, a))
    float scalar = dot_a_b / dot_a_a;
    return (vector){
        b.x - a.x * scalar,
        b.y - a.y * scalar
    };
}

