
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
#include "texture.h"
#include "sphere_tex.h"


#include "config.h"

/* Include headers for our objects. */

/* --------------------- */

GLfloat aspectRatio;

/* OpenGL specific variables */
GLuint vao;

/* Object instances */
GLuint drawmode = 0;
GLfloat modelScale;

Texture* terrainTex;
Terrain* terrain;
SkyBox* skyBox;
ShaderProgram* mainProgram;
ShaderProgram* skyboxShader;
Texture* sunTex;
Sphere* sun;

/* For time. */
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/* For camera. */
Camera cam(glm::vec3(0.0f, 0.0f, 10.0f));
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

	//skyBox = new SkyBox( skyBoxTextureFilePaths,"shaders\\skybox\\skybox.vert", "shaders\\skybox\\skybox.frag");

	// For creating terrain
	terrainTex = new Texture("textures\\grass.jpg");
	initTerrain();

	// Fon the sun
	sunTex = new Texture("textures\\sun.jpg");
	sun = new Sphere(true);
	sun->makeSphere(65,65);

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

	std::stack<glm::mat4> model;

	// Indentity matix
	model.push(glm::mat4(1.0f));

	//Translations which apply to all of out objects
	model.top() = glm::translate(model.top(), glm::vec3(0, 0, 0));
	//glUniformMatrix4fv(modelId, 1, GL_FALSE, &model.top()[0][0]);
	mainProgram->passMat4("model", model.top());

	// For drawing terrain
	terrainTex->bindTexture();
	terrain->drawObject(drawmode);

	sunTex->bindTexture();
	sun->drawSphere(0);

	model.pop();

	glUseProgram(0);

	// ------------------- Sky Box ------------------------
	//skyBox->draw(view, projection);
	// ----------------------------------------------------
}

///* This is the handler called for any key input. */
//void key_handler(GLFWwindow* window, int key, int s, int action, int mods) {
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//
//	/* Cycle between drawing vertices, mesh and filled polygons */
//	if (key == ',' && action != GLFW_PRESS)
//	{
//		drawmode++;
//		if (drawmode > 2) drawmode = 0;
//	}
//
//	if (key == 'W')
//		cam_z_inc -= CAM_MOVEMENT_SPEED; // Move forward
//	if (key == 'S')
//		cam_z_inc += CAM_MOVEMENT_SPEED; // Move backword
//	if (key == 'D')
//		cam_x_inc += CAM_MOVEMENT_SPEED; // Move right
//	if (key == 'A')
//		cam_x_inc -= CAM_MOVEMENT_SPEED; // Move left
//	if (key == GLFW_KEY_SPACE)
//		cam_y_inc += CAM_MOVEMENT_SPEED; // Move up
//	if (key == GLFW_KEY_LEFT_CONTROL)
//		cam_y_inc -= CAM_MOVEMENT_SPEED; // Move down
//
//	if (key == GLFW_KEY_RIGHT)
//		cam_angle_x_inc += CAM_MOVEMENT_SPEED; // Look right
//	if (key == GLFW_KEY_LEFT)
//		cam_angle_x_inc -= CAM_MOVEMENT_SPEED; // Look left
//	if (key == GLFW_KEY_UP)
//		cam_angle_y_inc += CAM_MOVEMENT_SPEED; // Look up
//	if (key == GLFW_KEY_DOWN)
//		cam_angle_y_inc -= CAM_MOVEMENT_SPEED; // Look down
//
//	if (key == ']') y_rotation_inc += 0.1f;
//	if (key == '[') y_rotation_inc -= 0.1f;
//}

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
		cam.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cam.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cam.processKeyboard(DOWN, deltaTime);
}

//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}

/* Mouse movement handler. */
void mouseHandler(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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
	mainProgram = new ShaderProgram("shaders\\main.vert","shaders\\main.frag");

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

