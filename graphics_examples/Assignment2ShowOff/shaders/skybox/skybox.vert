#version 420

layout(location = 0) in vec3 position;

uniform mat4 view, projection;

out vec3 ftexcoords;


void main() {
  ftexcoords = position;
  vec4 pos = projection * view * vec4(position, 1.0);

  gl_Position = pos.xyww;
}