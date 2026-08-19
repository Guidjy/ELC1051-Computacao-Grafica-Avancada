#ifndef VBOSPHERE_H
#define VBOSPHERE_H

#include "GL/glew.h"
#include "GLFW\glfw3.h"
#include "Scene.h"
#include <vector>
#include "glslprogram.h"

class Sphere : public Scene {
public:
	Sphere(GLFWwindow* window, int quality, vec3 spPos);
	
	// mesh virtual functions
	void init();
	void update(double t);
	void render();
	void resize(int, int);

private:
	void genSphere();
	void genBuffers();

	GLuint vaoID;
	int quality;
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec3> colors;
	std::vector<unsigned int> indices;
	
	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;

	vec3 lightPos;
	vec3 spherePos;
};

#endif