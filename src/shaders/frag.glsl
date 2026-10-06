#version 460 core
out vec4 FragColor;
uniform float someValue;
in vec3 pos;
void main()
{
    FragColor = vec4(pos.x, pos.y, pos.z, 1.0f);
}