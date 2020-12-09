/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation. */
#include "wrapper_glfw.h"
#include <iostream>

/* We'll use the STD stack class to make our stack or matrices. */
#include <stack>

/* Include GLM core and matrix extensions. */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Include headers for our objects. */
//#include "sphere.h"
#include "cube.h"

/* --------------------- */

/* CONSTANTS */
const float FOV = 60.0F;
const float CAM_MOVEMENT_SPEED = 0.5;
const float CUBE_RAW_WIDTH = 0.5;
const int CUBE_WIDTH = 3;

/* Used for resizing the window. */
GLfloat aspect_ratio;


/* OpenGL specific variables */

GLuint program, vao;


/* Shader variables (Unifoirms) */
GLuint modelId, viewId, projectionId;

/* Vertecises */
glm::vec3 cubePos;

/* Object instances */
GLuint drawmode=0;
GLfloat modelScale;
Cube cubes[CUBE_WIDTH][CUBE_WIDTH][CUBE_WIDTH];
int cubesSize; // Array length
float cubeGap = 0.1f; // Gaps inside the cube


/* Togglable settings variables */


/* Used for movement */
GLfloat cam_x_inc, cam_y_inc, cam_z_inc = 0; // For moving the position of the camera
GLfloat cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc = 0; // For moving the angle of the camera

GLfloat y_rotation, y_rotation_inc = 0 ;


/* Function that first sets up the scene. */
void init(GLWrapper *glw)
{
	//aspect_ratio = 1024.f / 768.f;
	aspect_ratio = 1920.0f / 1080.0f;
	modelScale = 3;
	cubePos = glm::vec3(-1,-1,-1);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	try
	{
		program = glw->LoadShader("shaders\\main.vert", "shaders\\main.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "SHADER LOAD EXCEPTION: " << e.what() << std::endl;
		std::cin.ignore();
		exit(1); // 1 to tell the system that there was an error
	}

	// Defining all the uniforms
	modelId = glGetUniformLocation(program, "model");
	viewId = glGetUniformLocation(program, "view");
	projectionId = glGetUniformLocation(program, "projection");
	cubesSize = sizeof(cubes) / sizeof(cubes[0]);

	for (int i = 0; i < CUBE_WIDTH; i++) // For every level of the cube
		for (int j = 0; j < CUBE_WIDTH; j++) // For ever row of the level
			for (int k = 0; k < CUBE_WIDTH; k++) // For each item in the row
				cubes[i][j][k].makeCube();

}

/* Function called for drawing every frame. */
void draw() {
	
	// ------------------- Basic setup -------------------

	// Background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the previous frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Define the shader program to be used
	glUseProgram(program);

	// ------------------ Basic setup end -------------------
	
	// ------------------ Setting up camera -------------------

	//Defining the projection model and passing it to the shader
	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspect_ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &projection[0][0]);

	// Defining the camera direction and passing it to the shader
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 8.0f);
	glm::vec3 camAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camHeadsup = glm::vec3(0.0f, 1.0f, 0.0f);  

	// Camera movement changes
	glm::vec3 camMoveBy = glm::vec3(cam_x_inc,cam_y_inc,cam_z_inc);
	glm::vec3 camLookMoveBy = glm::vec3(cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc) + camMoveBy;

	// To apply the changes
	camAngle = camLookMoveBy;
	camPosition += camMoveBy;

	/* 
	* TODO: 
	*	- Need to figure out how to move the camera towards the lookAt vector.
	*	- Need to figure out the angle max out issue.
	* 
	* Nice to have:
	*	- Add mouse input for angle change.
	*	- Add camera tilt (prefferably toggles).
	* 
	*/
	
	glm::mat4 view = glm::lookAt(camPosition, camAngle, camHeadsup);

	glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);

	// ------------------- Tralslations -------------------

	std::stack<glm::mat4> model;
	
	// Indentity matix
	model.push(glm::mat4(1.0f));

	// Translations which apply to all of out objects
	model.top() = glm::scale(model.top(), glm::vec3(modelScale)); // to scale equally in all axis
	model.top() = glm::translate(model.top(), cubePos);
	//model.top() = glm::translate(model.top(), cubePos * glm::vec3((CUBE_WIDTH / 2) * CUBE_RAW_WIDTH, (CUBE_WIDTH / 2) * CUBE_RAW_WIDTH, 0));
	//model.top() = glm::rotate(model.top(), glm::radians(20.0f), glm::vec3(1,0,0));

	// Translations which apply to each individual cube
	glm::vec3 levelSpacingVec = glm::vec3(0);
	glm::vec3 rowSpacingVec = glm::vec3(0);
	glm::vec3 itemSpacingVec = glm::vec3(0);

	// For every level of the cube
	for (int i = 0; i < CUBE_WIDTH; i++)
	{
		// Creating spacing for each level
		levelSpacingVec = cubePos * glm::vec3(0, CUBE_RAW_WIDTH * i, 0);

		// For ever row of the level
		for (int j = 0; j < CUBE_WIDTH; j++)
		{
			// Creating spacing for each row
			rowSpacingVec = cubePos * glm::vec3(-CUBE_RAW_WIDTH * j , 0, 0);
			
			// For each item in the row
			for (int k = 0; k < CUBE_WIDTH; k++)
			{
				model.push(model.top());

				// This rotates of the middle cube in the last layer
				//if (i == 0 /*&& !( j == 1 && k == 1)*/)
				//{
				//	model.top() = glm::rotate(model.top(), glm::radians(y_rotation), glm::vec3(0, 1, 0));
				//}

				// Moving the cube back
				itemSpacingVec = glm::vec3(0, 0, -CUBE_RAW_WIDTH * k);

				// Translate the model (and applying the gap)
				model.top() = glm::translate(model.top(), (itemSpacingVec + rowSpacingVec + levelSpacingVec) * glm::vec3(1+cubeGap));

				
				// Send model to shader
				glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));

				model.pop();

				// This rotates the rest of the cubes in the last layer
				if (i == 0  /*&& j == 1 && k == 1*/)
				{
					model.push(model.top());
					model.top() = glm::rotate(model.top(), glm::radians(y_rotation), glm::vec3(0, 1, 0));
					model.top() = glm::translate(model.top(),glm::vec3(-1.6,1,-1.6));
					model.top() = glm::translate(model.top(), (itemSpacingVec + rowSpacingVec + levelSpacingVec) * glm::vec3(1+cubeGap));
					glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));
					model.pop();
				}

				// Draw the cube
				cubes[i][j][k].drawCube(drawmode);
			}
		}
	}

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	// Modify animation vars
	y_rotation += y_rotation_inc;
}

/* This is the handler called for any key input. */
void key_handler(GLFWwindow* window, int key, int s, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (key == ',' && action != GLFW_PRESS)
	{
		drawmode++;
		if (drawmode > 2) drawmode = 0;
	}

	
	if (key == 'W') cam_z_inc -= 1.0f * CAM_MOVEMENT_SPEED; // Move forward
	if (key == 'S') cam_z_inc += 1.0f * CAM_MOVEMENT_SPEED; // Move backword
	if (key == 'D') cam_x_inc += 1.0f * CAM_MOVEMENT_SPEED; // Move right
	if (key == 'A') cam_x_inc -= 1.0f * CAM_MOVEMENT_SPEED; // Move left
	if (key == GLFW_KEY_SPACE) cam_y_inc += 1.0f * CAM_MOVEMENT_SPEED; // Move up
	if (key == GLFW_KEY_LEFT_CONTROL) cam_y_inc -= 1.0f * CAM_MOVEMENT_SPEED; // Move down
	
	if (key == GLFW_KEY_RIGHT) cam_angle_x_inc += 1.0f * CAM_MOVEMENT_SPEED; // Look right
	if (key == GLFW_KEY_LEFT) cam_angle_x_inc -= 1.0f * CAM_MOVEMENT_SPEED; // Look left
	if (key == GLFW_KEY_UP) cam_angle_y_inc += 1.0f * CAM_MOVEMENT_SPEED; // Look up
	if (key == GLFW_KEY_DOWN) cam_angle_y_inc -= 1.0f * CAM_MOVEMENT_SPEED; // Look down

	if (key == ']') y_rotation_inc += 0.1f;
	if (key == '[') y_rotation_inc -= 0.1f;


}

/* This is the hadnler called when the window is resised. */
void resise_handler(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspect_ratio = float(w) / float(h);
}

/* Entry point. */
int main(void) {
	//GLWrapper *glw = new GLWrapper(1024, 778, "Assignemt1");;
	GLWrapper *glw = new GLWrapper(1920, 1080, "Assignemt1");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	// Register the callback functions
	glw->setRenderer(draw);
	glw->setKeyCallback(key_handler);
	glw->setReshapeCallback(resise_handler);
	
	/* Output the OpenGL vendor and version */
	glw->DisplayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

