//#version 420

//in vec3 ftexcoords;
//out vec4 out_colour;

//uniform samplerCube skybox;
//void main() {
//  out_colour = texture(skybox, ftexcoords);
//}
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}