#include "PointSprites.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))

PointSprites::PointSprites(GLFWwindow* window, int numPoints)
{
	this->numPoints = numPoints;
	this->window = window;
	planePos = vec3(0.0f, 0.0f, 2.5f);
}

void PointSprites::init()
{
	genPoints();
	genBuffers();

	// init matrices
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, -1.0f), //eye
		vec3(0.0f, 0.0f, 0.0f), //center
		vec3(0.0f, 1.0f, 0.0f)); //up
	projectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);

	// load shaders
	try
	{
		shader.compileShader("shader/glsl40_point_sprites.vert", GLSLShader::VERTEX);
		shader.compileShader("shader/glsl40_point_sprites.geom", GLSLShader::GEOMETRY);
		shader.compileShader("shader/glsl40_point_sprites.frag", GLSLShader::FRAGMENT);

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) 
	{
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();
}

void PointSprites::update(double deltaTime)
{
	processInput();

	//// matrices setup
	modelMatrix = mat4(1.0f); // identity
	modelMatrix = glm::translate(modelMatrix, planePos); // translate back
	modelViewMatrix = viewMatrix * modelMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	// set var MVP on the shader
	shader.setUniform("ModelViewMatrix", modelViewMatrix); //ModelView
	shader.setUniform("ProjectionMatrix", projectionMatrix); //ModelView
}

void PointSprites::processInput()
{
	// toggle wireframe
	if (glfwGetKeyOnce(window, 'E')) {
		wireframe = !wireframe;
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void PointSprites::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, (GLubyte *)NULL);
	glBindVertexArray(0);
}

void PointSprites::genBuffers()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(0);  // Vertex position -> layout 0 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void PointSprites::resize(int x, int y)
{

}

void PointSprites::genPoints()
{
	// Random uniform distribution C++
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	// Generate random points
	for (size_t i = 0; i < numPoints; i++)
	{
		// Rand number between 0 and 1
		float x = distribution(generator);
		float y = distribution(generator);
		float z = distribution(generator);

		vertices.push_back(vec3(x, y, z));
	}

	// Generate indices
	for (size_t i = 0; i < numPoints; i++)
		indices.push_back(i);
}
