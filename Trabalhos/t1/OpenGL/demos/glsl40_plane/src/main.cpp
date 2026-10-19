/*
	GLSL 4.x demo
	Mostra o uso de:
	-GLM - Math library
	-VBO & VAO
	-GL error detection
	-glfwGetKeyOnce

	May 2016 - Tiago Augusto Engel - tengel@inf.ufsm.br
*/

//#define GLEW_STATIC

//Include GLEW - always first 
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <array>
#include <iostream>
#include "Quad.h"


#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	1000

auto quads = std::array<Quad*, 10>();
GLFWwindow* window;
bool isSRGBEnabled = false;
bool isShaderGammaCorrectionEnabled = false;

glm::mat4 projection;

//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY) (glfwGetKey(WINDOW, KEY) ? (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) : (keyOnce[KEY] = false))



void mainLoop()
{
	double lastTime = glfwGetTime();
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// toggle SRGB frame buffer
		if (glfwGetKeyOnce(window, 'Q')){
			isSRGBEnabled = !isSRGBEnabled;
			if (isSRGBEnabled)
			{
				glEnable(GL_FRAMEBUFFER_SRGB);
			}
			else
			{
				glDisable(GL_FRAMEBUFFER_SRGB);
			}
		}
		// toggle shader gamma correction
		if (glfwGetKeyOnce(window, 'E')) {
			isShaderGammaCorrectionEnabled = !isShaderGammaCorrectionEnabled;
		}

		for (auto quad : quads)
		{
			glm::mat4 MVP = projection * quad->model;
			quad->shader.setUniform("MVP", MVP);
			quad->shader.setUniform("isShaderGammaCorrectionEnabled", isShaderGammaCorrectionEnabled);

			quad->render();
		}

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

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Q: ativa framebuffer SRGB - E: ativa gamma correction via shader", NULL, NULL);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	// creates an orthographic viewing box stretching from -5 to 5 on both X and Y axes.
	projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);

	for (int i = 0; i < quads.size(); i++)
	{
		float brightness = (float)i / quads.size();
		float size = 1.0f;
		vec2 pos = vec2(-4.5f + (size * i), 0.0f);

		quads[i] = new Quad(size, pos, vec3(brightness));
	}

	mainLoop();

	exit(EXIT_SUCCESS);
}