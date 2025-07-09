#version 330 core

layout(location = 0) in vec2 aPos;
uniform vec3 inColor;
out vec3 color;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 camera;
void main() {

     gl_Position = projection *camera*translation* vec4(aPos, 0.0,1.0);
     color = inColor;
 };