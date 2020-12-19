#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY),
	zoom(ZOOM)
{
	pos = position;
	worldUp = up;
	yaw = yaw;
	pitch = pitch;
	updateVecs();
}

void Camera::processKeyboard(CameraMovement direction, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		pos += front * velocity;
	if (direction == BACKWARD)
		pos -= front * velocity;
	if (direction == LEFT)
		pos -= right * velocity;
	if (direction == RIGHT)
		pos += right * velocity;
	if (direction == UP)
		pos += up * velocity;
	if (direction == DOWN)
		pos += -up * velocity;
}

void Camera::processMouseMovement(float xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateVecs();
}


void Camera::processMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::updateVecs()
{
	// calculate the new Front vector
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(f);

	// re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

