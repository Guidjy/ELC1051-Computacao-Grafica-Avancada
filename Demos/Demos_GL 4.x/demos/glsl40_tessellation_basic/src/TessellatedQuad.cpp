#include "TessellatedQuad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;

//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))

TessellatedQuad::TessellatedQuad(GLFWwindow* window, int size)
{
	this->size = size;
	this->window = window;
	planePos = vec3(0.0f, 0.0f, 2.5f);
}

void TessellatedQuad::init()
{
	genPlane();
	genBuffers();

	// init matrices
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, -1.0f), //eye
		vec3(0.0f, 0.0f, 0.0f), //center
		vec3(0.0f, 1.0f, 0.0f)); //up
	projectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);

	// load shaders
	try {
		shader.compileShader("shader/glsl40_basic_tess.vert", GLSLShader::VERTEX);
		shader.compileShader("shader/glsl40_basic_tess.tcs", GLSLShader::TESS_CONTROL);
		shader.compileShader("shader/glsl40_basic_tess.tes", GLSLShader::TESS_EVALUATION);
		shader.compileShader("shader/glsl40_basic_tess.frag", GLSLShader::FRAGMENT);

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();

	outer = vec4(1.0f);
}

void TessellatedQuad::update(double deltaTime)
{
	processInput();

	//// matrices setup
	modelMatrix = mat4(1.0f); // identity
	modelMatrix = glm::translate(modelMatrix, planePos); // translate back
	modelViewMatrix = viewMatrix * modelMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	// set var MVP on the shader
	shader.setUniform("MVP", modelViewProjectionMatrix); //ModelViewProjection

	shader.setUniform("Inner", inner);
	shader.setUniform("Outer", outer);
}

void TessellatedQuad::processInput()
{
	// Inner tessellation 0
	if (glfwGetKeyOnce(window, 'Q'))
	{
		inner[0]++;
		if (inner[0] > 64)
			inner[0] = 64;
	}
	if (glfwGetKeyOnce(window, 'A'))
	{
		inner[0]--;
		if (inner[0] < 1)
			inner[0] = 1;
	}
	// Inner tessellation 0
	if (glfwGetKeyOnce(window, 'W'))
	{
		inner[1]++;
		if (inner[1] > 64)
			inner[1] = 64;
	}
	if (glfwGetKeyOnce(window, 'S'))
	{
		inner[1]--;
		if (inner[1] < 1)
			inner[1] = 1;
	}
	// Outer tessellation 0
	if (glfwGetKeyOnce(window, 'E'))
	{
		outer[0]++;
		if (outer[0] > 64)
			outer[0] = 64;
	}
	if (glfwGetKeyOnce(window, 'D'))
	{
		outer[0]--;
		if (outer[0] < 1)
			outer[0] = 1;
	}
	// Outer tessellation 1
	if (glfwGetKeyOnce(window, 'R'))
	{
		outer[1]++;
		if (outer[1] > 64)
			outer[1] = 64;
	}
	if (glfwGetKeyOnce(window, 'F'))
	{
		outer[1]--;
		if (outer[1] < 1)
			outer[1] = 1;
	}
	// Outer tessellation 2
	if (glfwGetKeyOnce(window, 'T'))
	{
		outer[2]++;
		if (outer[2] > 64)
			outer[2] = 64;
	}
	if (glfwGetKeyOnce(window, 'G'))
	{
		outer[2]--;
		if (outer[2] < 1)
			outer[2] = 1;
	}
	// Outer tessellation 3
	if (glfwGetKeyOnce(window, 'Y'))
	{
		outer[3]++;
		if (outer[3] > 64)
			outer[3] = 64;
	}
	if (glfwGetKeyOnce(window, 'H'))
	{
		outer[3]--;
		if (outer[3] < 1)
			outer[3] = 1;
	}
}

void TessellatedQuad::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (GLubyte *)NULL);
	glBindVertexArray(0);
}

void TessellatedQuad::genBuffers()
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

void TessellatedQuad::resize(int x, int y)
{

}

void TessellatedQuad::genPlane()
{
	// v0 -- bottom left
	vertices.push_back(vec3(-size, -size, 0.0f));

	//v1 -- top left
	vertices.push_back(vec3(-size, size, 0.0f));

	//v2 -- top right
	vertices.push_back(vec3(size, size, 0.0f));

	////v3 -- bottom right
	vertices.push_back(vec3(size, -size, 0.0f));
	
	// Quad indices
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
}
