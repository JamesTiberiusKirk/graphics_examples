#pragma once

#include <vector>
#include <string>

#include "common.h"
#include "texture.h"
#include "config.h"
#include "globals.h"


class SkyBox 
{
public:
	GLuint viewId, projectionId;
	Texture skyText;
	GLuint program, vao, vbo;

	SkyBox(const GLuint &program);
	~SkyBox();

	void init();
	void draw();
};

