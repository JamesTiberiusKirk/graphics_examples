#pragma once
#include <vector>
#include "common.h"
#include "program.h"
#include "texture.h"

/* Cubemap class for the skybox. */
class Skybox
{
private:
	CubemapTexture* faces;
	std::vector<std::string> facePaths;
	GLuint skyboxVAO;
    GLuint skyboxVBO;

public:
	ShaderProgram *skyboxShader;

	/* Constructor for initialisation. */
	Skybox(std::vector<std::string> fp, const char* vertPath, const char* fragPath);

	/* Called every frame. */
	void draw(glm::mat4 &view, glm::mat4 &projection);
};

