#pragma once

#pragma once

#include "texture.h"
#include <vector>
#include "common.h"

/* Refactored terrain class. */
class Terrain
{
public:
	Terrain(int octaves, GLfloat freq, GLfloat scale);
	~Terrain();

	void calculateNoise();
	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys, GLfloat sealevel=0);
	void calculateNormals();
	void stretchToRange(GLfloat min, GLfloat max);
	void setColour(glm::vec3 c);
	void setColourBasedOnHeight();
	void defineSeaLevel(GLfloat s);
	float heightAtPosition(GLfloat x, GLfloat z);
	glm::vec2 getGridPos(GLfloat x, GLfloat z);


	void createObject();
	void drawObject(int drawmode);

	glm::vec3 *vertices;
	glm::vec3 *normals;
	glm::vec3 *colours;
	std::vector<GLuint> elements;
	GLfloat* noise;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint vbo_mesh_colours;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colour;

	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlin_octaves;
	GLfloat perlin_freq;
	GLfloat perlin_scale;
	GLfloat height_scale;
	GLfloat sealevel;

	float height_min, height_max;	// range of terrain heights
};

