/*
GLSL 4.x demo
Mostra o uso de:
-GLM - Math library
-VBO & VAO
-GL error detection
-glfwGetKeyOnce
-toon shading 

May 2016 - Tiago Augusto Engel - tengel@inf.ufsm.br
*/


//Include GLEW - always first 
#include "GL/glew.h"
#include <GLFW/glfw3.h>

//Include the standard C++ headers 
#include "Scene.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include "Sphere.h"
#include <ctime>

#define WINDOW_WIDTH	600
#define WINDOW_HEIGHT	600


Scene *sphereA;
Scene *sphereB;
GLFWwindow* window;
bool wireframe = false;


//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))



void mainLoop()
{
	double thisTime;
	double lastTime = glfwGetTime();
	do
	{
		// toggle wireframe
		if (glfwGetKeyOnce(window, 'Q')) {
			wireframe = !wireframe;
			if (wireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set deltatime and call update
		thisTime = glfwGetTime();
		sphereA->update(thisTime - lastTime);
		sphereB->update(thisTime - lastTime);
		lastTime = thisTime;

		sphereA->render();
		sphereB->render();

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

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "glsl40_toon", NULL, NULL);
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
	// Initialize GLEW
	glewExperimental = GL_TRUE; //ensures that all extensions with valid entry points will be exposed.
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		system("pause");
		exit(EXIT_FAILURE);
	}
}
void initializeGL()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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
	srand((unsigned)time(NULL));
	sphereA = new Sphere(window, 100, glm::vec3(-1, 0, -5));
	sphereA->init();

	sphereB = new Sphere(window, 50, glm::vec3(1, 0, -5));
	sphereB->init();

	std::cout << std::endl << "Q: wireframe" << std::endl;

	std::cout << std::endl << "UP/DOWN: move esfera em Z" << std::endl;
	std::cout << std::endl << "LEFT/RIGHT: move lightPos em Z" << std::endl;

	mainLoop();

	exit(EXIT_SUCCESS);
}