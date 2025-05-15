#ifndef __MINKOWSKI__
#define __MINKOWSKI__
#include "../math/vector.h"

typedef struct{

    vector points[4];
    int side_counter;   
 

} Convex;

typedef struct{
//two points to put new point between if its A and B (0,1)
    int first;
    int second;
    vector normal;
   float depth;
}Edge;

void create_convex(Convex* convex){
    
     memset(convex->points,0,sizeof(Convex));
    convex->side_counter = 0;

}

#endif