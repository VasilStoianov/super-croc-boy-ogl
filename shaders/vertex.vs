#version 330 core

layout(location = 0) in vec3 aPos;
uniform vec3 pPos;

void main() {
    vec3 res = aPos +pPos;
     gl_Position = vec4(res, 1.0);
 };