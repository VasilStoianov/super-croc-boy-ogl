#version 330 core

layout(location = 0) in vec3 aPos;
uniform vec3 inColor;
out vec3 color;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 camera;
uniform mat4 shake;
void main() {
     mat4 cam = camera * shake;
     gl_Position = projection *cam*translation* vec4(aPos, 1.0);
     color = inColor;
 };