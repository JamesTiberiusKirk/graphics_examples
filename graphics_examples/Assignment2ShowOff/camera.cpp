#include "camera.h"

/*
TODO:
	- Ideally the aspect ratio should be static
	- Make a cam position switcher between different views
*/

/* Just a default constructor. */
Camera::Camera(Program *newPorgram)
	:	program(newPorgram)
{
	lookAt.position = glm::vec3(0.0f, 0.0f, 8.0f);
	lookAt.angle = glm::vec3(0.0f, 0.0f, 0.0f);
	lookAt.headsup = glm::vec3(0.0f, 1.0f, 0.0f);
	init();
}

/* Constructor with a custom cam vecs. */
Camera::Camera(Program *newProgram, const CameraVecs newCamVecs)
	:	program(newProgram),
		lookAt(newCamVecs)
{
	init();
}

/* This function initialises the uniforms */
void Camera::init()
{
	uniforms.view = glGetUniformLocation(program->uid, "view");
	uniforms.projection = glGetUniformLocation(program->uid, "projection");
}

/* The function which updates the cam every frame. */
void Camera::draw() 
{
	//Defining the projection model and passing it to the shader
	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
	glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, &projection[0][0]);

	lookAt.position = glm::vec3(0.0f, 0.0f, 8.0f);
	lookAt.angle = glm::vec3(0.0f, 0.0f, 0.0f);
	lookAt.headsup = glm::vec3(0.0f, 1.0f, 0.0f);


	lookAt.position += moveBy;
	lookAt.angle += lookMoveBy + moveBy;

	//std::cout << lookAt.position.x << std::endl;
	glm::mat4 view = glm::lookAt(lookAt.position, lookAt.angle, lookAt.headsup);
	glUniformMatrix4fv(uniforms.view, 1, GL_FALSE, &view[0][0]);
}

/* Funcion to move the camera postion and angle. */
void Camera::moveCam(glm::vec3 moveByVec, glm::vec3 angleByVec)
{
	moveBy = moveByVec;
	lookMoveBy = angleByVec;
}
