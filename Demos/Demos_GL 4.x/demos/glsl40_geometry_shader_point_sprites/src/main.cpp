// Geometry Shader Point Sprites
//
// Point sprites são quadriláteros simples que são alinhados de tal 
// modo que eles estão sempre de frente para a câmara. Este programa 
// é uma demonstração da geração de point sprites através do Geometry Shader.
// Vários pontos são enviados para GPU e, a partir de cada ponto, é gerado um 
// quadrad.
// O usuário pode alternar entre a wireframe e fill utilizando a tecla 'E'.
//
// Abril 2016 - Alex Frasson - afrasson@inf.ufsm.br

//Include GLEW - always first 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Include the standard C++ headers 
#include "GLUtils.h"
#include "Scene.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include "PointSprites.h"


#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	1000


using namespace std;

Scene *pointSprites;
GLFWwindow* window;


void mainLoop()
{
	double thisTime;
	double lastTime = glfwGetTime();
	do
	{
		// Check for OpenGL errors
		GLUtils::checkForOpenGLError(__FILE__, __LINE__);
		// set deltatime and call update
		thisTime = glfwGetTime();
		pointSprites->update(thisTime - lastTime);
		lastTime = thisTime;

		pointSprites->render();

		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));
}

// Define an error callback  
void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
// Key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Initialize GLFW  
void initGLFW()
{
	cout << "Initializing GLFW..." << endl;
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLSL4.0 + Point Sprites", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		system("pause");
		exit(EXIT_FAILURE);
	}
}
void initCallbacks()
{
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetErrorCallback(errorCallback);
}
void initGLEW()
{
	cout << "Initializing GLEW..." << endl;
	// Initialize GLEW
	glewExperimental = GL_TRUE; //ensures that all extensions with valid entry points will be exposed.
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		system("pause");
		exit(EXIT_FAILURE);
	}
	GLUtils::checkForOpenGLError(__FILE__, __LINE__); // Will throw error. Just ignore, glew bug.
	GLUtils::dumpGLInfo();
}
void initializeGL()
{
	cout << "Initializing GL..." << endl;
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLUtils::checkForOpenGLError(__FILE__, __LINE__);
}
// Close OpenGL window and terminate GLFW  
void closeApplication()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(void)
{
	initGLFW();
	initCallbacks();
	initGLEW();
	initializeGL();

	pointSprites = new PointSprites(window, 20);
	pointSprites->init();

	cout << endl << "E: toggle wireframe" << endl;

	mainLoop();

	exit(EXIT_SUCCESS);
}