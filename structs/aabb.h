#ifndef __AABB__
#define __AABB__
#include "../math/vector.h"

typedef struct {

  vector min;
  vector max;

} AABB;


void set_aabb(AABB* aabb, vector min, vector max){
    aabb->max = max;
    aabb->min = min;
}

#endif