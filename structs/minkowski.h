#ifndef __MINKOWSKI__
#define __MINKOWSKI__
#include "../math/vector.h"
#include "../structs/Stack.h"
typedef struct {

  // vector points[8];
  int side_counter;
  Stack points;

} Convex;

typedef struct {
  // two points to put new point between if its A and B (0,1)
  int first;
  int second;
  vector normal;
  float depth;
} Edge;

void create_convex(Convex *convex) {

  convex->side_counter = 0;
    create_stack(&(convex->points));
}

#endif