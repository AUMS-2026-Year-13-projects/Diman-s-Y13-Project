#version 460 core
layout (location = 0) in vec3 aPos;
out vec3 pos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main() {
    gl_Position = projection * view * model * vec4(aPos,1);
    // gl_Position = vec4(aPos.x, aPos.y+0.1f, aPos.z, 1.0);
    pos = aPos;
}