#ifndef UTILS_H
#define UTILS_H

float clamp(float value, float min, float max) {
  if (value > max)
    return max;
  if (value < min)
    return min;
  return value;
}




#endif