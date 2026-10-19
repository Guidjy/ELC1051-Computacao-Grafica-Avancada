#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "glslprogram.h"

class TessellatedQuad : public Scene
{
public:
	TessellatedQuad(GLFWwindow* window, int size = 1);

	// mesh virtual functions
	void init();
	void update(double t);
	void render();
	void resize(int, int);

private:
	void genPlane();
	void genBuffers();
	void processInput();

	GLuint vaoID;
	int size;
	std::vector<vec3> vertices;
	std::vector<vec2> texcoord;
	std::vector<unsigned int> indices;

	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;

	vec3 planePos;
	float rot = 30.0f;
	bool wireframe = false;

	int tessLevel = 1;
};