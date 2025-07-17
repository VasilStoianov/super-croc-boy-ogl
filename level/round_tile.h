#ifndef __ROUND__
#define __ROUND__
#include "../math/vector.h"
#include "../math/matrix.h"
#include "../structs/texture.h"
#include "math.h"
 typedef struct 
 {

    int radius;
    vector position;
    mat4f translate;
   float angle;
    Texture texture;  
    mat4f rotation;  
 }Round_Spike;


 Round_Spike* create_spike(int radius, vector position){
       Round_Spike* rs = (Round_Spike*) malloc(sizeof(Round_Spike));
     rs->angle = 0.0f;
       rs->position = position;
       rs->radius = radius;
       rs->rotation = identity();

       return rs;
       
 }

 void update_angle(float deltaTime,Round_Spike* rs)
{
  rs->angle += (float)(M_PI * deltaTime);
  if(rs->angle > 2 * M_PI){
     rs-> angle -= 2 * M_PI;
  }
  set_x_rotation(rs->angle,&(rs->rotation));
} 

#endif