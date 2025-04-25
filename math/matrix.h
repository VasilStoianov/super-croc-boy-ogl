#ifndef MATRIX_4F
#define MATRIX_4F
#include <string.h>
#include "vector.h"

typedef struct {
    float m[4][4];
} mat4f;


mat4f identity(){
    mat4f res = {0};
    memset(res.m,0.f,sizeof(mat4f));
    res.m[0][0] = res.m[1][1] = res.m[2][2] = res.m[3][3] = 1.f;

    return res;
}

void setTranslation(mat4f *matrix, vector vec ){

    matrix->m[3][0] = vec.x;
    matrix->m[3][1] = vec.y;
    matrix->m[3][2] = vec.z;
    
}

void scaleMatrix(mat4f *matrix, vector scale){
    matrix->m[0][0] = scale.x;
    matrix->m[1][1] = scale.y;
    matrix->m[2][2] = scale.z;
}

#endif