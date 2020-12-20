
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
//#include "skybox.h"
#include "program.h"
#include "texture.h"
#include "sphere_tex.h"
#include "cubemap.h"

#include "config.h"

/* --------------------- */

GLfloat aspectRatio;

/* OpenGL specific variables */
GLuint vao;

/* Object instances */
GLuint drawmode = 0;
GLfloat modelScale;

Texture* terrainTex;
Terrain* terrain;
//SkyBox* skyBox;
ShaderProgram* mainProgram;
ShaderProgram* skyboxShader;
Cubemap cubemap;
Texture* sunTex;
Sphere* sun;
Texture* airplaneTex;
TinyObjLoader* airplane;
GLfloat airplaneFlight = 0;
const GLfloat airplaneSpeed = 0.8f;

/* For time. */
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/* For camera. */
Camera cam(glm::vec3(0.0f, 3.0f, 8.0f));
GLfloat lastX = 1920.f / 2.f;
GLfloat lastY = 1080.f / 2.f;
bool firstMouse = true;

std::vector<std::string> skyBoxTextureFilePaths =
{
	"textures\\skybox\\right.jpg",
	"textures\\skybox\\left.jpg",
	"textures\\skybox\\top.jpg",
	"textures\\skybox\\bottom.jpg",
	"textures\\skybox\\front.jpg",
	"textures\\skybox\\back.jpg"
};

void initTerrain()
{
	terrain = new Terrain(3.0f, 1.0f, 6.0f);
	terrain->createTerrain(600.f, 600.f, 12.f, 12.f);
	terrain->defineSeaLevel(-5);
	terrain->setColourBasedOnHeight();
	//terrain->setColour(glm::vec3(0, 1, 1));
	terrain->createObject();
}

/* Function that first sets up the scene. */
void init(GLWrapper* glw)
{
	aspectRatio = 1920.0f / 1080.0f;
	modelScale = 3;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// For initialising the skybox
	//skyBox = new SkyBox( skyBoxTextureFilePaths,
	//	"shaders\\skybox\\skybox.vert", 
	//	"shaders\\skybox\\skybox.frag");
		//skyBox = new SkyBox();
	//skyBox->init(skyBoxProgram, skyBoxTextureFilePaths,
	//	"shaders\\skybox\\skybox.vert", 
	//	"shaders\\skybox\\skybox.frag");
	cubemap.init(skyboxShader);
	


	// For creating terrain
	terrainTex = new Texture("textures\\grass.jpg");
	initTerrain();

	// Fon the sun
	sunTex = new Texture("textures\\sun.jpg");
	sun = new Sphere(true);
	sun->makeSphere(65,65);


	// For the airplane
	//airplaneTex = new Texture("textures\\crumpled-paper.jpg");
	airplane = new TinyObjLoader();
	airplane->load_obj("obj\\paper-airplane-exp.obj");
}

/* Function called for drawing every frame. */
void draw() {
	// per-frame time logic
	// --------------------
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// ------------------- Basic setup -------------------

	// Background colour
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	// Clear the previous frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Define the shader program to be used
	mainProgram->use();

	// ------------------ Setting up camera -------------------

	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), aspectRatio, 0.1f, 100.0f);
	mainProgram->passMat4("view", view);
	mainProgram->passMat4("projection", projection);

	// ------------------- Tralslations -------------------

	//mainProgram->passVec3("light_colour",glm::vec3(1.0,1.0,1.0));
	//mainProgram->passVec3("object_colour", glm::vec3(1.0, 1.0, 1.0));

	mainProgram->passVec4("specular_colour", glm::vec4(0.0, 1.0, 1.0,1.0));
	mainProgram->passVec4("ambient_colour", glm::vec4(0.2));

	std::stack<glm::mat4> model;

	// Indentity matix
	model.push(glm::mat4(1.0f));

	//Translations which apply to all of out objects
	mainProgram->passMat4("model", model.top());

	// For drawing terrain
	terrainTex->bindTexture();
	terrain->drawObject(drawmode);
	terrainTex->unbindTexture();

	model.push(model.top());
	{
		model.top() = glm::rotate(model.top(), glm::radians(airplaneFlight), glm::vec3(0,1,0));
		model.top() = glm::translate(model.top(), glm::vec3(-3,2,0));
		model.top() = glm::scale(model.top(), glm::vec3(0.08f, 0.08f, 0.08f));
		model.top() = glm::rotate(model.top(), glm::radians(20.0f), glm::vec3(0,0,-1));
		
		mainProgram->passMat4("model", model.top());

		//mainProgram->passInt("tex_mode", 0);
		//airplaneTex->bindTexture();
		airplane->drawObject(0);
		//airplaneTex->unbindTexture();
	}
	model.pop();

	model.push(model.top());
	{
		glm::vec3 sunTranlsation = glm::vec3(-5, 5, -5);
		model.top() = glm::translate(model.top(), sunTranlsation);
		mainProgram->passMat4("model", model.top());
		//mainProgram->passInt("tex_mode", 1);

		sunTex->bindTexture();
		sun->drawSphere(0);
		sunTex->unbindTexture();
	}
	model.pop();
	

	glUseProgram(0);


	if (airplaneFlight == 360.0f) airplaneFlight = 0;
	airplaneFlight -= airplaneSpeed;

	// ------------------- Sky Box ------------------------
	//skyBox->draw(view, projection);
	cubemap.draw(view,projection);

	//glUseProgram(0);
	mainProgram->use();
	glDisableVertexAttribArray(0);
	// ----------------------------------------------------
}

/* Window resize handler. */
void resiseHandler(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspectRatio = float(w) / float(h);
}


/* Keyboard input handler. */
void keyHandler(GLFWwindow* window, int key, int s, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.processKey(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.processKey(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.processKey(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.processKey(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cam.processKey(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cam.processKey(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam.processKey(LOOK_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam.processKey(LOOK_DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam.processKey(LOOK_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.processKey(LOOK_LEFT, deltaTime);
}

/* Mouse movement handler. */
void mouseHandler(GLFWwindow* window, double xpos, double ypos)
{
	// To pause the mouse movement
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		return;
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	cam.processMouseMovement(xoffset, yoffset);
}

/* Mouse scroll handler. */
void scrollHandler(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.processMouseScroll(yoffset);
}


/* Entry point. */
int main(void) {
	GLWrapper* glw = new GLWrapper(1920, 1080, "Assignemt1");
	//mainProgram = new ShaderProgram("shaders\\main.vert","shaders\\main.frag");
	mainProgram = new ShaderProgram("shaders\\shader.vert","shaders\\shader.frag");

	skyboxShader = new ShaderProgram("shaders\\cubemap.vert","shaders\\cubemap.frag");

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	// Register the callback functions
	glw->setRenderer(draw);
	glw->setKeyCallback(keyHandler);
	glw->setMouseCallback(mouseHandler);
	glw->setScrollWheelCallback(scrollHandler);
	glw->setReshapeCallback(resiseHandler);

	/* Output the OpenGL vendor and version */
	glw->displayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

