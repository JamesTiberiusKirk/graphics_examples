// Minimal fragment shader
// for AC41001 Lab 5
// You should modify this fragment shader to apply texture
// appropriately

#version 400

in vec4 fcolour;
in vec2 fTextCoords;

uniform sampler2D text;

out vec4 outputColor;
void main()
{
	
	vec4 tecturecolour = texture(text, fTextCoords);

	outputColor = fcolour + tecturecolour;
}