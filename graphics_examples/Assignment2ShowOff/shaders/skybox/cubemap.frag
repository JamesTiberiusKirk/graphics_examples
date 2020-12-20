#version 330 core
out vec4 fragcolor;

in vec3 texcoords;

uniform samplerCube skybox;

void main()
{    
    fragcolor = texture(skybox, texcoords);
}