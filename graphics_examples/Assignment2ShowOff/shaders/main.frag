// Minimal fragment shader

#version 400 core

in vec4 frag_colour;
in vec3 frag_normal;
in vec3 frag_light_dir;
in vec3 frag_v;
in float frag_attenuation;
in vec2 ftextcoord;

out vec4 outputColor;

vec4 speculat_colour;
vec3 gloab_ambient;
vec3 emissive;

vec4 ambient_colour;
float shininess;

uniform sampler2D text1;

void main()
{
	speculat_colour = vec4(1.0f);
	shininess = 100.0f;
	gloab_ambient = vec3(0.5f);
	emissive = vec3(1.0f,1.0f,0.8f);
	ambient_colour = vec4(0.5);

	// For the ambient
	vec4 ambient = frag_colour * ambient_colour;


	// For the diffuse
	float normal_light = max(dot(frag_normal, frag_light_dir), 0);
	vec4 diffuse_light = frag_colour * normal_light;

	// Specular
	vec3 reflection = reflect(frag_light_dir, frag_normal);
	vec3 speculat_light = pow(max(dot(reflection,frag_v),0.0),shininess) * speculat_colour.xyz;
	
	// Tetxture
	vec4 textColour = texture(text1, ftextcoord);

	vec4 shaderColour = frag_attenuation * ( ambient + diffuse_light + vec4(speculat_light,1.0)); 

	outputColor = shaderColour;
	//outputColor = ambient_colour + textColour;
}