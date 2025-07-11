#ifndef __CIRCLE__
#define __CIRCLE__
#include "../math/vector.h"
#include "../math/matrix.h"

typedef struct 
{
    int radius;
    vector position;
    vector size; 
    mat4f translation; 
    vector color;
} Circle;

Circle* create_circle(int radius,vector position,vector size,vector color){
  Circle* circle_to_return =(Circle*) malloc(sizeof(Circle));
   circle_to_return->radius = radius;
   circle_to_return->position = position;
   circle_to_return-> size = size;
   circle_to_return->color = color;
   circle_to_return->translation = identity();
   setTranslation(&(circle_to_return->translation),circle_to_return->position);
   scaleMatrix(&(circle_to_return->translation),circle_to_return->size);
   return circle_to_return;
}


#endif