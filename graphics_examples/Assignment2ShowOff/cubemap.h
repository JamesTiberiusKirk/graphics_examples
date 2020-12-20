#pragma once
#include "common.h"
#include <vector>
#include "program.h"

class Cubemap
{
private:


	GLuint skyboxVAO;
    GLuint skyboxVBO;

	void initTexture();

public:
	ShaderProgram *skyboxShader;
	GLuint textureId;

	Cubemap() {};

	void init(ShaderProgram *shader);
	void draw(glm::mat4 &view, glm::mat4 &projection);
};

