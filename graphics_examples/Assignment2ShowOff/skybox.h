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
	Program *program;
	GLuint vao, vbo;

	SkyBox(const  std::vector<std::string> textureFiles, const char* vertShaderPath, const char* fragShaderPath);
	~SkyBox();

	void draw(glm::mat4 view, glm::mat4 projection);
};

