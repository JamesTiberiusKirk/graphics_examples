// Minimal fragment shader

#version 400 core

in vec3 frag_positon;
in vec3 frag_normal;
in vec2 frag_texcoord;


out vec4 outputColor;

uniform vec3 light_pos;
uniform vec3 light_colour;
uniform vec3 object_colour;
uniform int tex_mode;

uniform sampler2D tex1;

void main()
{
	// Ambient
	float ambient_strength = 0.5;
	vec3 ambient = ambient_strength * light_pos;

	// Diffuse
	vec3 normal = normalize(frag_normal);
	vec3 light_dir = normalize(light_pos - frag_positon);
	float diffuse_strength = max(dot(normal,light_dir), 0.0);
	vec3 diffuse = diffuse_strength * light_colour;

	// Texture
	vec4 texcolour = texture(tex1, 2* frag_texcoord);

	// Combine
	vec4 shader_colour = texcolour * vec4(ambient + diffuse,1.0);
	// vec3 result = (ambient + (texcolour * diffuse)) /** object_colour*/;
	// if(tex_mode== 0)
	// {
	// 	outputColor =  texcolour;
	// }
	// else if (tex_mode == 1)
	// {
	// 	outputColor = vec4(result,1.0) /** texcolour*/;
	// }

	outputColor =  shader_colour;

}