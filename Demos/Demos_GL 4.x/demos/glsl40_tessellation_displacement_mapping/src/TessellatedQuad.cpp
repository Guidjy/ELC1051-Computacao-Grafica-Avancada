#include "TessellatedQuad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "TextureManager.h"
#include "GLUtils.h"

using namespace std;

//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))

TextureManager* texManager;

TessellatedQuad::TessellatedQuad(GLFWwindow* window, int size)
{
	this->size = size;
	this->window = window;
	planePos = vec3(0.0f, 0.0f, 1.0f);
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
	
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	projectionMatrix = glm::perspective(glm::radians(75.0f), (float)w/(float)h, 0.1f, 100.0f);

	// load shaders
	try {
		shader.compileShader("shader/glsl40_tess_disp_mapp.vert", GLSLShader::VERTEX);
		shader.compileShader("shader/glsl40_tess_disp_mapp.tcs", GLSLShader::TESS_CONTROL);
		shader.compileShader("shader/glsl40_tess_disp_mapp.tes", GLSLShader::TESS_EVALUATION);
		shader.compileShader("shader/glsl40_tess_disp_mapp.frag", GLSLShader::FRAGMENT);

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();
	
	// Get a TextureManager's instance
	texManager = TextureManager::Inst();

	// Load our color texture with Id 0
	glActiveTexture(GL_TEXTURE0);
	if (!texManager->LoadTexture("..\\..\\resources\\old_bricks_sharp_diff_COLOR.png", 0))
		cout << "Failed to load texture." << endl;
	
	// Load our displacement texture with Id 1
	glActiveTexture(GL_TEXTURE1);	
	if (!texManager->LoadTexture("..\\..\\resources\\old_bricks_sharp_diff_DISP.png", 1))
		cout << "Failed to load texture." << endl;
}

void TessellatedQuad::update(double deltaTime)
{
	processInput();

	//// matrices setup
	modelMatrix = mat4(); // identity
	modelMatrix = glm::translate(modelMatrix, planePos); // translate back
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rot), vec3(0.0f, 1.0f, 0.0f));
	
	modelViewMatrix = viewMatrix * modelMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	// set var MVP on the shader
	shader.setUniform("MVP", modelViewProjectionMatrix); //ModelViewProjection

	shader.setUniform("TessLevel", tessLevel);

	shader.setUniform("displacementmapSampler", 1);
	shader.setUniform("colorTextureSampler", 0);
}

void TessellatedQuad::processInput()
{
	// Tessellation Level
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		tessLevel++;
		if (tessLevel > 64)
			tessLevel = 64;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		tessLevel--;
		if (tessLevel < 1)
			tessLevel = 1;
	}

	// Rotation on Y axis
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rot+=0.01;
		if (rot > 360)
			rot = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		rot-=0.01;
		if (rot < 0)
			rot = 360;
	}

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

void TessellatedQuad::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//// Bind both textures
	//// Color
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(0);
	//// Displacement
	glActiveTexture(GL_TEXTURE1);
	TextureManager::Inst()->BindTexture(1);
	
	glBindVertexArray(vaoID);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (GLubyte *)NULL);
	glBindVertexArray(0);
}

void TessellatedQuad::genBuffers()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(0);  // VertexPosition -> layout 0 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, texcoord.size() * sizeof(vec2), (GLvoid*)&texcoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(1);  // TexCoord -> layout 1 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
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
	texcoord.push_back(vec2(0.0f, 0.0f));

	//v1 -- top left
	vertices.push_back(vec3(-size, size, 0.0f));
	texcoord.push_back(vec2(0.0f, 1.0f));

	//v2 -- top right
	vertices.push_back(vec3(size, size, 0.0f));
	texcoord.push_back(vec2(1.0f, 1.0f));

	////v3 -- bottom right
	vertices.push_back(vec3(size, -size, 0.0f));
	texcoord.push_back(vec2(1.0f, 0.0f));
	
	// Quad indices
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
}
