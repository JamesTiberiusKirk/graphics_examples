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
#include "sphere.h"
#include "cube.h"
#include "cylinder.h"

/* --------------------- */

/* CONSTANTS */
const float FOV = 30.0F;
const float CAM_MOVEMENT_SPEED = 0.5;
const int SPHERE_COUNT = 5;
const int ANKER_POINT = 10;
const GLfloat SWING_ANGLE_MAX = 60.0f;
const int STRING_COUNT = SPHERE_COUNT;
const int CAMERA_MODE_COUNT = 3;

/* Used for resizing the window. */
GLfloat aspect_ratio;

/* OpenGL specific variables */
GLuint program, vao;

/* Shader variables (Unifoirms) base_component*/
GLuint modelId, viewId, projectionId;

/* Vertecises */
glm::vec3 spherePos;

/* Object instances */
GLuint drawmode = 0;
GLfloat modelScale;
Sphere spheres[SPHERE_COUNT];
int lats, longs;
GLfloat firstSwingSpeed = 0.0;
GLfloat lastSwingSpeed = 0.0;
GLfloat swingSpeed = 0.5;

Cube topRail;
Cube sideRails[2];

Cylinder cables[STRING_COUNT];

/* Togglable settings variables */
int cameraMode = 1;

/* Used for movement */
GLfloat cam_x_inc, cam_y_inc, cam_z_inc = 0;				   // For moving the position of the camera
GLfloat cam_angle_x_inc, cam_angle_y_inc, cam_angle_z_inc = 0; // For moving the angle of the camera

float firstZrotation = 0, lastZrotation = 0;

/* Used for the floor*/
GLuint vbo, nbo, cbo;
GLuint attrib_v, attrib_n, attrib_c;

/* To print help */
void printHelp()
{
	char msg[] = "\
	~~~~~~~~~~~~~~~~~~~~ KEY BINDINGS ~~~~~~~~~~~~~~~~~~~~~~\n \
	ESC:\t\t to exit  \n \
	\"W\"\t\t to move camera positively on the z axis\n\
	\"S\"\t\t to move camera negatively on the z axis\n\
	\"D\"\t\t to move camera positively on the x axis\n\
	\"A\"\t\t to move camera negatively on the x axis \n\
	SPACE\t\t to move camera positively on the y axis \n\
	LEFT CTRL\t to move camera negatively on the y axis\n\
	RIGHT ARROW\t to increment the look angle to the right\n\
	LEFT ATRROW\t to increment the look angle to the left\n\
	UP ARROW\t to increment the look angle to the up\n\
	DOWN ARROW\t to increment the look angle to the down\n\
	\"=\"\t\t to speed up the animation\n\
	\"-\"\t\t to slow down the animation\n\
	\"C\"\t\t to change the position of the camera\n\
	\",\"\t\t to change the drawing mode\n\
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	std::cout<<msg<<std::endl;
}

/* To create the floor rectangle */
void createFloor()
{
	// Defining attributes
	attrib_v = 0;
	attrib_n = 1;
	attrib_c = 2;

	// Defining the default vertex positions
	GLfloat vertexPositions[] =
	{
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f,
	};

	// Defining the normals
	GLfloat normalPositions[] =
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};

	// Definig the default colour
	float colourPositions[] = 
	{
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};

	// Setting the verticies
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Setting the normals
	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalPositions), normalPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Setting colours
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourPositions), colourPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* To draw the floor rectangle */
void drawFloor()
{
	// Binding vertex shape
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attrib_v);
	glVertexAttribPointer(attrib_v, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Bidning colours
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glEnableVertexAttribArray(attrib_c);
	glVertexAttribPointer(attrib_c, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Binding normals 
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glEnableVertexAttribArray(attrib_n);
	glVertexAttribPointer(attrib_n, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Drawing in triagle mode
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


/* To draw the cables */
void drawCables(const glm::mat4& model, Cylinder& cable, const GLfloat zRotation)
{
	std::stack<glm::mat4> cableModel;
	cableModel.push(glm::mat4(1.0f));

	glm::vec3 scaleVec = glm::vec3(0.03f, 10, 0.03f);

	cableModel.top() *= model;

	// translate
	cableModel.top() = glm::translate(cableModel.top(), glm::vec3(0, ANKER_POINT / 2, 0));

	// scale
	cableModel.top() = glm::scale(cableModel.top(), scaleVec);

	glUniformMatrix4fv(modelId, 1, GL_FALSE, &(cableModel.top()[0][0]));
	cable.drawCylinder(drawmode);
	cableModel.pop();
}

/* To change camera position */
glm::mat4 changeCam(const int mode)
{
	//Defining the projection model and passing it to the shader
	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspect_ratio, 0.1f, 100.0f);
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

/* Clear camera increment vars */
void clearCamInc()
{
	cam_x_inc = 0;
	cam_y_inc = 0;
	cam_z_inc = 0;

	cam_angle_x_inc = 0;
	cam_angle_y_inc = 0;
	cam_angle_z_inc = 0;
}

/* Function that first sets up the scene. */
void init(GLWrapper* glw)
{
	//aspect_ratio = 1024.f / 768.f;
	aspect_ratio = 1920.0f / 1080.0f;
	modelScale = 0;
	spherePos = glm::vec3(0);
	lats = 90;
	longs = 90;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	try
	{
		program = glw->LoadShader("shaders\\main.vert", "shaders\\main.frag");
		glCompileShader(program);
	}
	catch (std::exception& e)
	{
		std::cout << "SHADER LOAD EXCEPTION: " << e.what() << std::endl;
		std::cin.ignore();
		exit(1); // 1 to tell the system that there was an error
	}

	// Defining all the uniforms
	modelId = glGetUniformLocation(program, "model");
	viewId = glGetUniformLocation(program, "view");
	projectionId = glGetUniformLocation(program, "projection");

	for (int i = 0; i < SPHERE_COUNT; i++)
		spheres[i].makeSphere(lats, longs);

	topRail.makeCube();

	for (int i = 0; i < STRING_COUNT; i++)
		cables[i].makeCylinder();

	for (int i = 0; i < 2; i++)
		sideRails[i].makeCube();

	createFloor();

	// Setting up the first ball begining speed
	firstSwingSpeed = swingSpeed;
}

/* Function called for drawing every frame. */
void draw()
{

	// ------------------- Basic setup -------------------

	// Background colour
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Clear the previous frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Define the shader program to be used
	glUseProgram(program);

	// ------------------ Basic setup end -------------------

	// Setting up camera
	glm::mat4 view = changeCam(cameraMode);
	glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);

	// ------------------- Tralslations -------------------

	std::stack<glm::mat4> model;

	// Indentity matix
	model.push(glm::mat4(1.0f));

	// TO translate the floor
	{
		model.push(model.top());

		// tranlsate
		model.top() = glm::translate(model.top(), glm::vec3(5,-2,0));


		// scale
		model.top() = glm::scale(model.top(), glm::vec3(80, 1, 30));

		glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));
		drawFloor();
		model.pop();
	}
	
	// Translations which apply to all of out objects
	model.top() = glm::translate(model.top(), spherePos);


	glm::vec3 railSpacingVec = glm::vec3(5, ANKER_POINT, 0);
	// Translating the top rail
	{
		model.push(model.top());

		// tranlsate
		model.top() = glm::translate(model.top(), railSpacingVec);

		// scale
		model.top() = glm::scale(model.top(), glm::vec3(65, 1, 1));

		glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));
		model.pop();
		topRail.drawCube(drawmode);
	}



	glm::vec3 sideRailSpacingVec = glm::vec3(-10,(ANKER_POINT/2)-1,0);
	// Translating the side rails
	{
		model.push(model.top());

		// translate
		model.top() = glm::translate(model.top(), sideRailSpacingVec );

		// scale
		model.top() = glm::scale(model.top(),glm::vec3(1,25,1));

		glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));
		sideRails[0].drawCube(drawmode);

		// same for the second side rail
		model.top() = glm::translate(model.top(), glm::vec3(30,0,0));
		glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));
		sideRails[1].drawCube(drawmode);
		model.pop();
	}

	// Translating the spheres
	glm::vec3 sphereSpacingVec = glm::vec3(0);
	for (int i = 0; i < SPHERE_COUNT; i++)
	{
		model.push(model.top());
		sphereSpacingVec = glm::vec3(2 * i, 0, 0);
		
		//model.top() = glm::scale(model.top(), glm::vec3(2));
		if (!(i == 0) && !(i == SPHERE_COUNT - 1)) // For all the balls in-between (which do not move)
		{
			// Moving the origin
			model.top() = glm::translate(model.top(), sphereSpacingVec);

			// Drawing the calbes
			drawCables(model.top(), cables[i], 0);
		}
		else if (i == 0) // For the first ball
		{
			// Moving the origin
			model.top() = glm::translate(model.top(), sphereSpacingVec + glm::vec3(0, ANKER_POINT, 0));
			// Rotating it
			model.top() = glm::rotate(model.top(), -glm::radians(firstZrotation), glm::vec3(0, 0, 1));
			// Moving it to where it needs to be
			model.top() = glm::translate(model.top(), glm::vec3(0, -ANKER_POINT, 0));

			// Drawing the calbes
			drawCables(model.top(), cables[i], -firstZrotation);
		}
		else if (i == SPHERE_COUNT - 1) // For the last ball
		{
			// Moving the origin
			model.top() = glm::translate(model.top(), sphereSpacingVec + glm::vec3(0, ANKER_POINT, 0));
			// Rotating it
			model.top() = glm::rotate(model.top(), -glm::radians(-lastZrotation), glm::vec3(0, 0, 1));
			// Moving it to where it needs to be
			model.top() = glm::translate(model.top(), glm::vec3(0, -ANKER_POINT, 0));

			// Drawing the calbes
			drawCables(model.top(), cables[i], firstZrotation);
		}

		glUniformMatrix4fv(modelId, 1, GL_FALSE, &(model.top()[0][0]));
		spheres[i].drawSphere(drawmode);
		model.pop();
	}

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	// ------------------- Animation -------------------

	// Aplying the first ball swing
	firstZrotation += firstSwingSpeed;

	// Checking if the first ball needs to reverce or stop and start the last ball
	if (firstZrotation >= SWING_ANGLE_MAX)
		firstSwingSpeed = -swingSpeed;
	if (firstZrotation < -0.1f && (lastSwingSpeed == 0))
	{
		firstSwingSpeed = 0;
		lastSwingSpeed = swingSpeed;
	}

	// Applying the last ball swing
	lastZrotation += lastSwingSpeed;

	// Checking if the last ball needs to reverce or stop and start the first ball
	if (lastZrotation >= SWING_ANGLE_MAX)
		lastSwingSpeed = -swingSpeed;
	if (lastZrotation < -0.1f && (firstSwingSpeed == 0))
	{
		lastSwingSpeed = 0;
		firstSwingSpeed = swingSpeed;
	}
}

/* This is the handler called for any key input. */
void key_handler(GLFWwindow* window, int key, int s, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (key == ',' && action != GLFW_PRESS)
	{
		drawmode++;
		if (drawmode > 2)
			drawmode = 0;
	}

	// Cycle between camera angles and clear increment vars
	if (key == 'C' && action != GLFW_PRESS)
	{
		clearCamInc();
		cameraMode++;
		if (cameraMode > CAMERA_MODE_COUNT)
			cameraMode = 1;
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

	if (key == '=') // To make the annimation faster
	{
		if (swingSpeed > 4)
			swingSpeed = 0;
		
		swingSpeed += 0.1;
	}
	if (key == '-') // To make the annimation slower
	{
		if (swingSpeed < 0)
			swingSpeed = 3;
		swingSpeed -= 0.1;
	}
}

/* This is the hadnler called when the window is resised. */
void resise_handler(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspect_ratio = float(w) / float(h);
}

/* Entry point. */
int main(void)
{
	GLWrapper* glw = new GLWrapper(1920, 1080, "Assignemt1");

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

	// To print the key bindings
	printHelp();

	glw->eventLoop();

	delete (glw);
	return 0;
}
