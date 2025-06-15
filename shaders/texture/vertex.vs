#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTextCoord;
out vec3 color;
out vec2 TextCoord;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 camera;
void main() {
     mat4 cam = camera;
     gl_Position = projection *cam*translation* vec4(aPos, 1.0);
    TextCoord = aTextCoord;
 };