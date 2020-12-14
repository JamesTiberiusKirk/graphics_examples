#version 420

in vec3 ftexcoords;
out vec4 out_colour;

uniform samplerCube skybox;
void main() {
  out_colour = texture(skybox, ftexcoords);
}