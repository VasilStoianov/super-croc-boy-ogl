#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 translation;
uniform mat4 projection;
uniform mat4 camera;
uniform mat4 rotation;
void main() {
    TexCoord = aTexCoord;
    mat4 trans =   translation * rotation ;
    gl_Position = projection * camera * trans * vec4(aPos, 0.0, 1.0);
}