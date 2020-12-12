
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

#include "tiny_loader_texture.h"
#include "texture.h"
#include "cube_tex.h"
#include "terrain.h"

#include "config.h"
#include "globals.h"
#include "camera.h"


/* Include headers for our objects. */

/* --------------------- */

/* OpenGL specific variables */

GLuint program, vao;


/* Shader variables (Unifoirms) */
GLuint modelId, viewId, projectionId;

/* Vertecises */

/* Object instances */
GLuint drawmode = 0;
GLfloat modelScale;


/* Togglable settings variables */


/* Used for movement */
GLfloat cam_x_inc, cam_y_inc, cam_z_inc = 0; // For moving the position of the camera
GLfloat cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc = 0; // For moving the angle of the camera

GLfloat y_rotation, y_rotation_inc = 0;

TinyObjLoader revolver;
Cube cube;
Texture grassText;
Terrain* terrain;
//Camera* cam;
int cameraMode = 1; //Temporary untill i fix the camera class

void initTerrain()
{

	terrain = new Terrain(3.0f, 1.0f, 6.0f);
	terrain->createTerrain(600.f, 600.f, 12.f, 12.f);
	terrain->setColour(glm::vec3(0, 1, 1));
	terrain->createObject();
}


/* To change camera position */
glm::mat4 changeCam(const int mode)
{
	//Defining the projection model and passing it to the shader
	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &projection[0][0]);

	// Definng the default camera position
	glm::vec3 camPosition = glm::vec3(4.0f, 3.0f, 40.0f);
	glm::vec3 camAngle = glm::vec3(4.0f, 3.0f, 0.0f);
	glm::vec3 camHeadsup = glm::vec3(0.0f, 1.0f, 0.0f);

	switch (mode)
	{
	case 1:
		camPosition = glm::vec3(4.0f, 3.0f, 40.0f);
		camAngle = glm::vec3(4.0f, 3.0f, 0.0f);
		camHeadsup = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 2:
		camPosition = glm::vec3(4.0f, 50.0f, 0.0f);
		camAngle = glm::vec3(4.0f, 3.0f, 0.0f);
		camHeadsup = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	case 3:
		camPosition = glm::vec3(30.0f, 30.0f, 30.0f);
		camAngle = glm::vec3(2.0f, 3.0f, 0.0f);

		camHeadsup = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	}

	// Camera movement changes
	glm::vec3 camMoveBy = glm::vec3(cam_x_inc, cam_y_inc, cam_z_inc);
	glm::vec3 camLookMoveBy =
		glm::vec3(cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc)
		+ camMoveBy + camAngle;

	// To apply the changes
	camPosition += camMoveBy;
	camAngle = camLookMoveBy;

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

	return glm::lookAt(camPosition, camAngle, camHeadsup);
}

/* Function that first sets up the scene. */
void init(GLWrapper* glw)
{
	//aspect_ratio = 1024.f / 768.f;
	aspectRatio = 1920.0f / 1080.0f;
	modelScale = 3;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	try
	{
		program = glw->LoadShader("shaders\\main.vert", "shaders\\main.frag");
	}
	catch (std::exception& e)
	{
		std::cout << "SHADER LOAD EXCEPTION: " << e.what() << std::endl;
		std::cin.ignore();
		exit(1); // 1 to tell the system that there was an error
	}

	// Defining all the uniforms
	modelId = glGetUniformLocation(program, "model");

	// Cam setup
	viewId = glGetUniformLocation(program, "view");
	projectionId = glGetUniformLocation(program, "projection");
	//cam = new Camera(program);

	// For creating terrain
	initTerrain();

	//revolver = TinyObjLoader();
	//revolver.load_obj("obj\\revolver_normal.obj");

	// loading texture
	try
	{
		grassText.initTexture("textures\\wood.jpg");
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.ignore();
		exit(1); // 1 to tell the system that there was an error
	}

}

/* Function called for drawing every frame. */
void draw() {

	// ------------------- Basic setup -------------------

	// Background colour
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	// Clear the previous frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Define the shader program to be used
	glUseProgram(program);

	// ------------------ Setting up camera -------------------
	// THIS SETUP ISNT FINISHED

	/*
	// Camera movement changes
	cam->setAspectRatio(aspectRatio);

	// Camera movement changes
	glm::vec3 camMoveBy = glm::vec3(cam_x_inc, cam_y_inc, cam_z_inc);
	glm::vec3 camLookMoveBy =
		glm::vec3(cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc)
		+ camMoveBy + cam->lookAt.angle;

	cam->changeCam(camMoveBy, camLookMoveBy);

	cam->draw();
	*/

	// Setting up camera
	glm::mat4 view = changeCam(cameraMode);
	glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);

	// ------------------- Tralslations -------------------

	std::stack<glm::mat4> model;

	// Indentity matix
	model.push(glm::mat4(1.0f));

	// Translations which apply to all of out objects
	//model.top() = glm::scale(model.top(), glm::vec3(modelScale)); // to scale equally in all axis
	model.top() = glm::translate(model.top(), glm::vec3(0, -1, 0));
	//model.top() = glm::rotate(model.top(), glm::radians(20.0f), glm::vec3(1,0,0));
	glUniformMatrix4fv(modelId, 1, GL_FALSE, &model.top()[0][0]);


	grassText.bindTexture();
	//revolver.drawObject(drawmode);
	// For drawing terrain
	terrain->drawObject(drawmode);
	grassText.unbindTexture();


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


	if (key == 'W')
		cam_z_inc -= CAM_MOVEMENT_SPEED; // Move forward
	if (key == 'S')
		cam_z_inc += CAM_MOVEMENT_SPEED; // Move backword
	if (key == 'D')
		cam_x_inc += CAM_MOVEMENT_SPEED; // Move right
	if (key == 'A')
		cam_x_inc -= CAM_MOVEMENT_SPEED; // Move left
	if (key == GLFW_KEY_SPACE)
		cam_y_inc += CAM_MOVEMENT_SPEED; // Move up
	if (key == GLFW_KEY_LEFT_CONTROL)
		cam_y_inc -= CAM_MOVEMENT_SPEED; // Move down

	if (key == GLFW_KEY_RIGHT)
		cam_angle_x_inc += CAM_MOVEMENT_SPEED; // Look right
	if (key == GLFW_KEY_LEFT)
		cam_angle_x_inc -= CAM_MOVEMENT_SPEED; // Look left
	if (key == GLFW_KEY_UP)
		cam_angle_y_inc += CAM_MOVEMENT_SPEED; // Look up
	if (key == GLFW_KEY_DOWN)
		cam_angle_y_inc -= CAM_MOVEMENT_SPEED; // Look down

	if (key == ']') y_rotation_inc += 0.1f;
	if (key == '[') y_rotation_inc -= 0.1f;


}

/* This is the hadnler called when the window is resised. */
void resise_handler(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspectRatio = float(w) / float(h);
}

/* Entry point. */
int main(void) {
	//GLWrapper *glw = new GLWrapper(1024, 778, "Assignemt1");;
	GLWrapper* glw = new GLWrapper(1920, 1080, "Assignemt1");;

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

