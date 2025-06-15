#ifndef __vertices__
#define __vertices__
const float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.5f,  0.5f, 0.0f,
  -0.5f,  0.5f, 0.0f
};

const float texture_vertices[] = {
  -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
   0.5f, -0.5f, 0.0f, 1.0f ,0.0f,
   0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, 0.0f, 0.0f, 1.0f 
};
const unsigned int indices[] = {  
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 
 
#endif
