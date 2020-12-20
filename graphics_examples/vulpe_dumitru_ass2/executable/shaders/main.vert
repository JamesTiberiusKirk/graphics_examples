#version 420

/* These are the vertex attributes. */
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

/* Uniform variables are passed in from the application. */
uniform mat4 model, view, projection;
uniform vec4 ambient_colour;

vec4 lightpos = vec4(-5,5,-5,1);

/* Out vars for frag. */
out float fattenuation;
out vec3 fV, fnormal, f_lightdir;
out vec2 ftexcoords;
out vec4 fcolour;


/* Function to compute attenuation. */
float compute_attenuation(vec3 L)
{
	float attenuation;
	float distance_to_light = length(L);
	
	float k1 = 0.5;
	float k2 = 0.5;
	float k3 = 0.5;

	attenuation = 1.0 / (k1 + k2 * distance_to_light + k3 * pow(distance_to_light, 2));
	return 40*attenuation;
}

void main()
{
	// Defining the model view.
	mat4 modelview = view * model; 
	
	// transform the vertex positions into modelview
	vec4 P = modelview * vec4(position, 1.0); 
	
	
	vec3 L = lightpos.xyz - P.xyz; 

	// transform the normal to the modelview coordinates
	mat3 normal_transformation = transpose(inverse(mat3(modelview)));
	vec3 N = normal_transformation * normal;

	// Normalisations out
	fnormal = normalize(N);
	f_lightdir = normalize(L);
	fV = normalize(-P.xyz);
	
	// Attenuation out
	fattenuation = compute_attenuation(L);

	// Sending out the ambient colour as fcolour.
	fcolour = ambient_colour;
	
	// Texture coordinates out.
	ftexcoords = vec2(texcoords.x, texcoords.y);

	// Define the vertex position
	gl_Position = projection * view * model * vec4(position,1.0);
}