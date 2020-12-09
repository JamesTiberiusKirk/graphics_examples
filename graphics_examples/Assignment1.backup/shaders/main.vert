#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
//uniform uint colourmode;
//uniform uint lightmode;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;

void main()
{
	vec4 diffuse_colour;
	vec4 position_h = vec4(position, 1.0);
	diffuse_colour = vec4(0.0, 1.0, 0, 1.0);

	/* Begining of lab 3 code */
	// Step 2 creating ambient light
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0) * diffuse_colour;
	
	// Step 3 creating defusal light
	vec3 light_dir = vec3(0,1,0);
	mat4 model_view = view * model;

	vec4 position4 = vec4(position,1);
	mat3 normal_matrix = transpose(inverse(mat3(model_view)));

	vec3 N = normal * normal_matrix;

	vec3 light_dir_normal = normalize(light_dir);
	vec3 N_normal = normalize(N);

	float diffuse = max(dot(N_normal, light_dir_normal),0);

	vec4 diffuse_lighting = diffuse * diffuse_colour;
	
	// Step 4 creating specular light
	vec4 p = model * view * position_h;
	float s = 8.0;
	vec3 sc = vec3(1,1,1);

	vec3 V = normalize(vec3(p));
	vec3 R = reflect(-light_dir,N_normal);

	vec4 specular = vec4(pow(max(dot(R, V), 0.0), s) * sc, 1);

	// Step 5 Creating emmisive lighting
	vec4 emissive_colour = vec4(0,1,0,1);


	// Define the vertex colour
	fcolour = ambient + diffuse_lighting + specular /*+ emissive_colour*/;

	/* End of lab 3 code */

	// Define the vertex position
	gl_Position = projection * view * model * position_h;
}

