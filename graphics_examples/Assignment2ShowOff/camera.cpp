#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat nyaw, GLfloat npitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	zoom(ZOOM)
{
	pos = position;
	worldUp = up;
	yaw = nyaw;
	pitch = npitch;
	updateVecs();
}

/* For WASD keys. */
void Camera::processKey(CameraMovement type, GLfloat deltaTime)
{
	GLfloat velocity = SPEED * deltaTime;
	if (type == FORWARD)
		pos += front * velocity;
	if (type == BACKWARD)
		pos -= front * velocity;
	if (type == LEFT)
		pos -= right * velocity;
	if (type == RIGHT)
		pos += right * velocity;
	if (type == UP)
		pos += up * velocity;
	if (type == DOWN)
		pos += -up * velocity;
}

/* For arrow keys; need to update cam verticies. */
void Camera::processKey(CameraLook type, GLfloat deltaTime)
{
	GLfloat velocity = SPEED * deltaTime;
	if (type == LOOK_UP)
		processMouseMovement(0.f, ARROW_LOOK_SENSITIVITY);
	if (type == LOOK_DOWN)
		processMouseMovement(0.f, -ARROW_LOOK_SENSITIVITY);
	if (type == LOOK_LEFT)
		processMouseMovement(-ARROW_LOOK_SENSITIVITY, 0.f);
	if (type == LOOK_RIGHT)
		processMouseMovement(ARROW_LOOK_SENSITIVITY, 0.f);

	updateVecs();
}


void Camera::processMouseMovement(float xoffset, GLfloat yoffset)
{
	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Update front, right and up vectors using the updated Euler angle.
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
	// Calculate the new Front vector.
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(f);

	// Re-calculate the right and up vector.
	right = glm::normalize(glm::cross(front, worldUp)); 
	up = glm::normalize(glm::cross(right, front));
}

