#pragma once

/* Include GLM core and matrix extensions. */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "wrapper_glfw.h"
#include "config.h"
#include "program.h"

struct CameraUniforms {
	GLuint view;
	GLuint projection;
};

struct CameraVecs {
	glm::vec3 position;
	glm::vec3 angle;
	glm::vec3 headsup;
};

class Camera
{
public:

	glm::vec3 moveBy;
	glm::vec3 lookMoveBy;
	CameraVecs lookAt;
	CameraUniforms uniforms;
	GLfloat aspectRatio;
	Program *program;

	Camera(Program *newProgram);
	Camera(Program *newProgram, const CameraVecs newCamVecs);
	~Camera() {};

	void init();
	void draw();

	void setAspectRatio(const GLfloat nAspectRatio)
	{
		this->aspectRatio = nAspectRatio;
	}

	void moveCam(const glm::vec3 moveByVec, const glm::vec3 angleByVec);
};

