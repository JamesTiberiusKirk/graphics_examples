#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 textcoord;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 frag_colour;
out vec3 frag_normal;
out vec3 frag_light_dir;
out vec3 frag_v;
out float frag_attenuation;
out vec2 ftextcoord;

void main()
{
	vec4 position_h = vec4(position, 1.0f);
	vec4 light_pos = vec4(3,3,3,1.0f);

	mat4 model_view = view * model;
	vec4 position_vert_model_view = model_view * position_h;
	vec3 light_point = light_pos.xyz - position_vert_model_view.xyz;

	mat3 normal_transform = transpose(inverse(mat3(model_view)));
	vec3 normal_normal = normal_transform * normal;

	frag_normal = normalize(normal_normal);
	frag_light_dir = normalize(light_point);
	
	float light_distance = length(light_point);

	frag_colour = vec4(colour,1.0);
	frag_v = normalize(-light_point.xyz);
	frag_attenuation = 1.0 / (0.001 + 0.001 * light_distance + 0.001 *pow(light_distance, 2));

	gl_Position = projection * view * model * position_h;

	ftextcoord = textcoord;
}

