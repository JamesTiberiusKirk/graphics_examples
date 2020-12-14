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
	Texture skyText;
	Camera *cam;
	Program *program;
	GLuint vao, vbo;

	SkyBox(const char* vertShaderPath, const char* fragShaderPath);
	~SkyBox();

	void draw();
};

