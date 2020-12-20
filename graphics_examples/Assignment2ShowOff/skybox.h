#pragma once

#include <vector>
#include <string>

#include "common.h"
#include "texture.h"
#include "config.h"
#include "camera.h"
#include "program.h"

class SkyBox 
{
public:
	GLuint viewId, projectionId;
	CubemapTexture *skyTex;
	Camera *cam;
	ShaderProgram program;
	GLuint vao, vbo;
	GLuint texId;

	SkyBox();
	~SkyBox();

	void init(ShaderProgram &p, const std::vector<std::string> textureFilesPath,
		const char* vertShaderPath,
		const char* fragShaderPath);
	void draw(glm::mat4 &view, glm::mat4 &projection);
};

