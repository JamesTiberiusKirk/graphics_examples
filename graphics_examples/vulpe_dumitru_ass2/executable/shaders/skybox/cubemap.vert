#version 330 core
layout (location = 0) in vec3 position;

out vec3 texcoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texcoords = position;
    gl_Position = projection * view * vec4(position, 1.0);
} 