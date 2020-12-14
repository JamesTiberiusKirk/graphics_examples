
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

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stack>

#include "tiny_loader_texture.h"
#include "wrapper_glfw.h"
#include "terrain.h"
#include "camera.h"
#include "skybox.h"
#include "program.h"

#include "config.h"

/* Include headers for our objects. */

/* --------------------- */

// TODO NEED TO MAKE THIS STATIC GLOBALLY SOMEHOW
GLfloat aspectRatio;

/* OpenGL specific variables */
GLuint vao;

/* Shader variables (Unifoirms) */
GLuint modelId, viewId, projectionId;

/* Object instances */
GLuint drawmode = 0;
GLfloat modelScale;

Terrain* terrain;
Camera* cam;
SkyBox* skyBox;
Program* mainProgram;


/* Used for movement */
GLfloat cam_x_inc, cam_y_inc, cam_z_inc = 0; // For moving the position of the camera
GLfloat cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc = 0; // For moving the angle of the camera

GLfloat y_rotation, y_rotation_inc = 0;


std::vector<std::string> skyBoxTextureFilePaths =
{
	"textures\\cubemap\\right.jpg",
	"textures\\cubemap\\left.jpg",
	"textures\\cubemap\\top.jpg",
	"textures\\cubemap\\bottom.jpg",
	"textures\\cubemap\\front.jpg",
	"textures\\cubemap\\back.jpg"
};

void initTerrain()
{
	terrain = new Terrain(3.0f, 1.0f, 6.0f, "textures\\earth.png");
	terrain->createTerrain(600.f, 600.f, 12.f, 12.f);
	terrain->setColour(glm::vec3(0, 1, 1));
	terrain->createObject();
}

/* Function that first sets up the scene. */
void init(GLWrapper* glw)
{
	aspectRatio = 1920.0f / 1080.0f;
	modelScale = 3;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Defining all the uniforms
	modelId = glGetUniformLocation(mainProgram->uid, "model");
	// For the Cam setup
	cam = new Camera(mainProgram);


	skyBox = new SkyBox( skyBoxTextureFilePaths,"shaders\\skybox\\skybox.vert", "shaders\\skybox\\skybox.frag");

	// For creating terrain
	initTerrain();
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
	glUseProgram(mainProgram->uid);

	// ------------------ Setting up camera -------------------

	// Camera movement changes
	cam->setAspectRatio(aspectRatio);
	//Camera::setAspectRatio(aspectRatio);

	// Camera movement changes
	glm::vec3 camMoveBy = glm::vec3(cam_x_inc, cam_y_inc, cam_z_inc);
	glm::vec3 camLookMoveBy = glm::vec3(cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc);
	cam->moveCam(camMoveBy, camLookMoveBy);
	cam->draw();

	// ------------------- Tralslations -------------------

	std::stack<glm::mat4> model;

	// Indentity matix
	model.push(glm::mat4(1.0f));

	// Translations which apply to all of out objects
	model.top() = glm::translate(model.top(), glm::vec3(0, -1, 0));
	glUniformMatrix4fv(modelId, 1, GL_FALSE, &model.top()[0][0]);

	// For drawing terrain
	terrain->drawObject(drawmode);

	glUseProgram(0);

	// Modify animation vars
	y_rotation += y_rotation_inc;

	// ------------------- Sky Box ------------------------

	glDepthFunc(GL_LEQUAL);
	glUseProgram(skyBox->program->uid);

	skyBox->draw();
	glDisableVertexAttribArray(0);

	glUseProgram(0);
	glDepthFunc(GL_LESS);

	// ----------------------------------------------------
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
	GLWrapper* glw = new GLWrapper(1920, 1080, "Assignemt1", "shaders\\main.vert","shaders\\main.frag");
	mainProgram = glw->program;

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
	glw->displayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

