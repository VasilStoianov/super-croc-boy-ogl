#ifndef __MINKOWSKI__
#define __MINKOWSKI__
#include "../math/vector.h"

typedef struct{

    vector points[4];
    int side_counter;   
 

} Convex;

void create_convex(Convex* convex){
    
     memset(convex->points,0,sizeof(Convex));
    convex->side_counter = 0;

}

#endif