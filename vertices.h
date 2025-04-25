#ifndef __vertices__
#define __vertices__
const float vertices[] = {
     0.375f,  0.5f, 0.0f,  // top right
     0.375f, -0.5f, 0.0f,  // bottom right
    -0.375f, -0.5f, 0.0f,  // bottom left
    -0.375f,  0.5f, 0.0f   // top left 
};
const unsigned int indices[] = {  
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 
 
#endif
