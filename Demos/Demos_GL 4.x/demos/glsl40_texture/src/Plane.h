#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "glslprogram.h"
#include "TextureManager.h"

class Plane : public Scene {
public:
	Plane(GLFWwindow* window, int size = 1);

	// mesh virtual functions
	void init();
	void update(double t);
	void render();
	void resize(int, int);

private:
	void genPlane();
	void genBuffers();
	void loadTextures();

	GLuint vaoID;
	int size;
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> texUV;
	std::vector<unsigned int> indices;

	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;

	vec3 planePos;
};