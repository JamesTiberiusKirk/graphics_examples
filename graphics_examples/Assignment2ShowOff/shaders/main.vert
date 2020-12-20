#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
// layout(location = 3) in vec3 colour;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
// uniform uint colourmode;

// Output the vertex colour - to be rasterized into pixel fragments

out vec3 frag_positon;
out vec3 frag_normal;
out vec2 frag_texcoord;

void main()
{
	frag_positon = vec3(model * vec4(position,1.0));
	frag_normal = inverse(transpose(mat3(model))) * normal;

	gl_Position = projection * view * vec4(frag_positon,1.0);
	
	frag_texcoord = texcoord;
}

