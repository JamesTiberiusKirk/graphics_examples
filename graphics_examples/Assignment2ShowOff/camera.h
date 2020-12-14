#pragma once

/* Include GLM core and matrix extensions. */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Inlcude GL_Load and GLFW */
/*
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>
*/
#include "wrapper_glfw.h"
#include "config.h"
#include <iostream>
//#include "globals.h"

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

	//glm::mat4 projection;

	glm::vec3 moveBy;
	glm::vec3 lookMoveBy;

	CameraVecs lookAt;
	CameraUniforms uniforms;

	GLfloat aspectRatio;
	GLuint program;


	Camera(const GLuint& newProgram);
	Camera(const GLuint& newProgram, const CameraVecs newCamVecs);
	~Camera() {};


	void init();
	void draw();

	void setAspectRatio(const GLfloat newAspectRatio);

	void changeCam(const glm::vec3 moveByVec, const glm::vec3 angleByVec);

	void movePosBy(const glm::vec3 moveByVec);
	void angleBy(const glm::vec3 angleByVec);
};

