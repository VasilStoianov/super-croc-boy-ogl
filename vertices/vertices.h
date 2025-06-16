#ifndef __vertices__
#define __vertices__
const float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.5f,  0.5f, 0.0f,
  -0.5f,  0.5f, 0.0f
};

const float texture_vertices[] = {
  // x,     y,    z,    u,    v
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // Bottom Left
   0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // Bottom Right
   0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  // Top Right
  -0.5f,  0.5f, 0.0f, 0.0f, 0.0f   // Top Left
};
const unsigned int indices[] = {  
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 
 
const unsigned int indices2[] = {  
    0, 1, 2,   // first triangle
    2, 3, 0    // second triangle
}; 

#endif
