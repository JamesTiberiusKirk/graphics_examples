#include "camera.h"

/*
TODO:
	- need to figure out how to increment the camera angle and not throw it in a direction
	- need to figure out hwo to increment the camera movement and not throw it again
	- want to make camera position (like in the last assignment)
*/

Camera::Camera(const GLuint &newPorgram)
	:	program(newPorgram)
{
	lookAt.position = glm::vec3(0.0f, 0.0f, 8.0f);
	lookAt.angle = glm::vec3(0.0f, 0.0f, 0.0f);
	lookAt.headsup = glm::vec3(0.0f, 1.0f, 0.0f);
	init();
}

Camera::Camera(const GLuint &newProgram, const CameraVecs newCamVecs)
	:	program(newProgram),
		lookAt(newCamVecs)
{
	init();
}

void Camera::init()
{
	uniforms.view = glGetUniformLocation(program, "view");
	uniforms.projection = glGetUniformLocation(program, "projection");
}

void Camera::draw() 
{
	//Defining the projection model and passing it to the shader
	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
	glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, &projection[0][0]);

	lookAt.position = glm::vec3(0.0f, 0.0f, 8.0f);
	lookAt.angle = glm::vec3(0.0f, 0.0f, 0.0f);
	lookAt.headsup = glm::vec3(0.0f, 1.0f, 0.0f);


	lookAt.position += moveBy;
	lookAt.angle = lookMoveBy;

	std::cout << lookAt.position.x << std::endl;
	glm::mat4 view = glm::lookAt(lookAt.position, lookAt.angle, lookAt.headsup);
	glUniformMatrix4fv(uniforms.view, 1, GL_FALSE, &view[0][0]);
}

void Camera::movePosBy(const glm::vec3 moveByVec) 
{
	lookAt.position += moveByVec;
}

void Camera::angleBy(glm::vec3 angleByVec) 
{
	lookAt.angle = angleByVec;
}

void Camera::changeCam(glm::vec3 moveByVec, glm::vec3 angleByVec)
{
	//lookAt.position += moveByVec;
	//lookAt.angle = angleByVec;

	moveBy = moveByVec;
	lookMoveBy = angleByVec;
}

void Camera::setAspectRatio(const GLfloat newAspectRatio) 
{
	aspectRatio = newAspectRatio;
}
