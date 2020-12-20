#pragma once

#include <iostream>

#include "common.h"
#include "wrapper_glfw.h"
#include "config.h"
#include "program.h"

/* Camera movement options. */
enum CameraMovement 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/* Camera look change options. */
enum CameraLook 
{
    LOOK_UP,
    LOOK_DOWN,
    LOOK_RIGHT,
    LOOK_LEFT
};

/* Defaults. */
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ARROW_LOOK_SENSITIVITY = 50.0f * SENSITIVITY;
const GLfloat ZOOM = 45.0f;

/* Class for doing all of the camera calculations. */
class Camera
{

private:
    /* Camera Attributes. */
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    /* Euler Angles. */
    GLfloat yaw;
    GLfloat pitch;

    /* Calculate the eulers angles for FPS movement. */
    void updateVecs();

public:

    GLfloat zoom;

    /* Default constructor with vectors. */
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat nyaw = YAW,
        GLfloat npitch = PITCH);

    /* Generate lookAt matrix. */
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(pos, pos + front, up);
    }

    /* Input handlers*/
    void processKey(CameraMovement type, GLfloat deltaTime);
    void processKey(CameraLook type, GLfloat deltaTime);
    void processMouseMovement(float xoffset, GLfloat yoffset);
    void processMouseScroll(float yoffset);

};
