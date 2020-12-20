#version 420

/* Some defaults. */
vec3 global_ambient = vec3(0.05, 0.05, 0.05);
float shininess = 8.0;

/* In vars from vert. */
in vec4 fcolour;
in float fattenuation;
in vec2 ftexcoords;
in vec3 fV, fnormal, f_lightdir;

/* Uniforms. */
uniform sampler2D tex1;
uniform vec4 specular_colour;

out vec4 output_colour;

void main()
{
	vec3 emissive = vec3(0);

	// Calculating diffuse lighting
	vec4 diffuse_colour = vec4(0.6,0.6,0.6,1.0);
	float NL = max(dot(fnormal, f_lightdir),0);
	vec4 diffuse_lighting = NL * diffuse_colour;

	// Calculating specular lighting
	vec3 R = reflect(-f_lightdir, fnormal);
	vec3 specular_lighting = pow(max(dot(R, fV), 0.0), shininess) * specular_colour.xyz;

		
	// Calculating the texture colour.
	vec4 texcolour = texture(tex1, ftexcoords);

	// Mixting all the colours and lightings.
	vec4 shadedcolour = texcolour*(fcolour + diffuse_lighting) + vec4(specular_lighting, 1.0);

	// Passing oputput colour to frame buffer.
	output_colour = (fattenuation)*(shadedcolour) + vec4(emissive, 1.0) + vec4(global_ambient, 1.0);
}