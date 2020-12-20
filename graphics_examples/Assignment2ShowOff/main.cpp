
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
#include "program.h"
#include "texture.h"
#include "sphere_tex.h"
#include "skybox.h"

/* ------- PATHS -------- */

std::vector<std::string> CUBEMAP_FACE_PATHS =
{
	"textures\\skybox\\right.jpg",
	"textures\\skybox\\left.jpg",
	"textures\\skybox\\top.jpg",
	"textures\\skybox\\bottom.jpg",
	"textures\\skybox\\front.jpg",
	"textures\\skybox\\back.jpg"
};

const char* TEXT_GRASS = "textures\\grass.jpg";
const char* TEXT_SUN = "textures\\sun.jpg";
const char* TEXT_PAPER = "textures\\crumpled-paper.jpg";

const char* CUBEMAP_VERT_PATH = "shaders\\skybox\\cubemap.vert";
const char* CUBEMAP_FRAG_PATH = "shaders\\skybox\\cubemap.frag";

const char* MAIN_VERT_PATH = "shaders\\main.vert";
const char* MAIN_FRAG_PATH = "shaders\\main.frag";

const char* OBJ_AIRPLANE = "obj\\paper-airplane-exp.obj"; 


/* ----- OTHER CONSTANTS -- */

/* ------------------------ */

int view = 0;
GLfloat aspectRatio;

/* OpenGL specific variables */
GLuint vao;

/* Object instances */
GLuint drawmode = 0;
GLfloat modelScale;

Texture* terrainTex;
Terrain* terrain;

ShaderProgram* mainProgram;
Skybox* cubemap;
Texture* sunTex;
Sphere* sun;

Texture* airplaneTex;
TinyObjLoader* airplane;
GLfloat airplaneFlight = 0;
GLfloat airplaneSpeed = 0.8f;

/* For time. */
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/* For camera. */
Camera cam(glm::vec3(0.0f, 3.0f, 8.0f));
GLfloat lastX = 1920.f / 2.f;
GLfloat lastY = 1080.f / 2.f;
bool firstMouse = true;

/* To print help */
void printHelp()
{
	char msg[] = "\
	~~~~~~~~~~~~~~~~~~~~ KEY BINDINGS ~~~~~~~~~~~~~~~~~~~~~~\n \
	ESC:\t\t to exit the application  \n \
	\"W\"\t\t to move forward\n\
	\"S\"\t\t to move backwords\n\
	\"D\"\t\t to move right\n\
	\"A\"\t\t to move left\n\
	SPACE\t\t to move upwards\n\
	LEFT CTRL\t to move downwards\n\
	RIGHT ARROW\t to look right\n\
	LEFT ATRROW\t to look left\n\
	UP ARROW\t to look up\n\
	DOWN ARROW\t to look down\n\
	\"=\"\t\t to speed up the animation\n\
	\"-\"\t\t to slow down the animation\n\
	\"C\"\t\t to change the position of the camera\n\
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	std::cout << msg << std::endl;
}

/* To initialise defaults for the terrain. */
void initTerrain()
{
	terrain = new Terrain(3.0f, 1.0f, 6.0f);
	terrain->createTerrain(600.f, 600.f, 12.f, 12.f);
	terrain->defineSeaLevel(-5);
	terrain->setColourBasedOnHeight();
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
	cubemap = new Skybox(CUBEMAP_FACE_PATHS,
		CUBEMAP_VERT_PATH,
		CUBEMAP_FRAG_PATH);
	
	// For creating terrain
	terrainTex = new Texture(TEXT_GRASS);
	initTerrain();

	// Fon the sun
	sunTex = new Texture(TEXT_SUN);
	sun = new Sphere(true);
	sun->makeSphere(65,65);


	// For the airplane
	airplaneTex = new Texture(TEXT_PAPER);
	airplane = new TinyObjLoader();
	airplane->load_obj(OBJ_AIRPLANE);
}

/* Function called for drawing every frame. */
void draw() {

	// ------------------- Basic setup -------------------

	// Logic to get time difference
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Background colour
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	// Clear the previous frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Define the shader program to be used
	mainProgram->use();

	// Pass some deafaults to the shader
	mainProgram->toUniform("specular_colour", glm::vec4(0.0, 1.0, 1.0,1.0));
	mainProgram->toUniform("ambient_colour", glm::vec4(0.2));

	// ------------------ Setting up camera -------------------

	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), aspectRatio, 0.1f, 100.0f);
	mainProgram->toUniform("view", view);
	mainProgram->toUniform("projection", projection);

	// ------------------- Tralslations -------------------

	std::stack<glm::mat4> model;

	// Indentity matix
	model.push(glm::mat4(1.0f));

	//Translations which apply to all of out objects
	mainProgram->toUniform("model", model.top());

	// For drawing terrain
	terrainTex->bindTexture();
	terrain->drawObject(drawmode);
	terrainTex->unbindTexture();

	// Fir the airplane
	model.push(model.top());
	{
		model.top() = glm::rotate(model.top(), glm::radians(airplaneFlight), glm::vec3(0,1,0));
		model.top() = glm::translate(model.top(), glm::vec3(-3,2,0));
		model.top() = glm::scale(model.top(), glm::vec3(0.08f, 0.08f, 0.08f));
		model.top() = glm::rotate(model.top(), glm::radians(20.0f), glm::vec3(0,0,-1));
		
		mainProgram->toUniform("model", model.top());

		airplaneTex->bindTexture();
		airplane->drawObject(0);
		airplaneTex->unbindTexture();
	}
	model.pop();

	// For the sun
	model.push(model.top());
	{
		glm::vec3 sunTranlsation = glm::vec3(-5, 5, -5);
		model.top() = glm::translate(model.top(), sunTranlsation);
		mainProgram->toUniform("model", model.top());

		sunTex->bindTexture();
		sun->drawSphere(0);
		sunTex->unbindTexture();
	}
	model.pop();
	

	glUseProgram(0);

	// ------------------- Plane Animation ----------------
	if (airplaneFlight == 360.0f) airplaneFlight = 0;
	airplaneFlight -= airplaneSpeed;
	// ----------------------------------------------------


	// ------------------- Sky Box ------------------------
	cubemap->draw(view,projection);
	// ----------------------------------------------------

	mainProgram->use();
	glDisableVertexAttribArray(0);
}

/* Window resize handler. */
void resiseHandler(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspectRatio = float(w) / float(h);
}

/* To change airplane speed. */
void changeAirplaneSpeed(GLfloat change)
{
	if ( (change < 0 && airplaneSpeed <= 0.4) || (change > 0 && airplaneSpeed >= 3.0) )
		return;
	airplaneSpeed += change;
}


/* Cycles through camera views. */
void changeView()
{
	view++;
	if (view >= 3)
		view = 0;

	switch (view)
	{
	case 0:
		cam = Camera(glm::vec3(0,20,0),glm::vec3(0,1,0),-90,-90);
		break;
	case 1:
		cam = Camera(glm::vec3(6),glm::vec3(0,1,0),-120,-25);
		break;
	case 2:
		cam = Camera(glm::vec3(15,1,0),glm::vec3(0,1,0),-180,0);
		break;
	case 3:
		cam = Camera(glm::vec3(0.0f, 3.0f, 8.0f));
			break;
	}

}

/* Keyboard input handler. */
void keyHandler(GLFWwindow* window, int key, int s, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (key == 'C' && action != GLFW_PRESS)
		changeView();

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		changeAirplaneSpeed(0.4);
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		changeAirplaneSpeed(-0.4);

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
	printHelp();

	GLWrapper* glw = new GLWrapper(1920, 1080, "Assignemt1");
	mainProgram = new ShaderProgram(MAIN_VERT_PATH, MAIN_FRAG_PATH);

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

