#ifndef UTILS_H
#define UTILS_H

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


#endif