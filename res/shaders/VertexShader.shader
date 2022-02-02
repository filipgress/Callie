#version 410

layout (location = 0) in vec3 pos;
out vec4 vCol;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 setColor;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vCol = setColor;
}