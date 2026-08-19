#include "Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;

Plane::Plane(GLFWwindow* window, int size){
	this->size = size;
	this->window = window;
	planePos = vec3(0.0f, 0.0f, 2.5f);

	// init matrices
	modelMatrix = glm::translate(planePos);
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, 1.0f), //eye
		vec3(0.0f, 0.0f, -1.0f), //center
		vec3(0.0f, 1.0f, 0.0f)); //up
	viewMatrix *= glm::scale(vec3(1.0f, 1.0f, -1.0f));

	projectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
	// load shaders
	try {
		shader.compileShader("shader/glsl40_texture.vert");
		shader.compileShader("shader/glsl40_texture.frag");

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();
	glEnable(GL_CULL_FACE);
}

void Plane::loadTextures(){
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->LoadTexture("..\\..\\resources\\lena.bmp", 1);
	glActiveTexture(GL_TEXTURE1);
	TextureManager::Inst()->LoadTexture("..\\..\\resources\\textura.bmp", 2);
}

void Plane::init(){
	genPlane();
	genBuffers();
	loadTextures();
}

void Plane::update(double deltaTime){
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f), vec3(0.0f, 1.0f, 0.0f));
	}
	//// matrices setup
	modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	// set var MVP on the shader
	shader.setUniform("MVP", modelViewProjectionMatrix); //ModelViewProjection
}

void Plane::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLubyte *)NULL);
	glBindVertexArray(0);
}

void Plane::genBuffers(){

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(0);  // Vertex position -> layout 0 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, texUV.size() * sizeof(vec2), (GLvoid*)&texUV[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // texture coords -> layout 1 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Plane::resize(int x, int y){

}

void Plane::genPlane(){

	// v0 -- top left
	vertices.push_back(vec3(-size, size, 0.0f));
	texUV.push_back(vec2(0.0f, 1.0f));

	 //v1 -- bottom left
	vertices.push_back(vec3(-size, -size, 0.0f));
	texUV.push_back(vec2(0.0f, 0.0f));
	
	// v2 -- bottom right
	vertices.push_back(vec3(size, -size, 0.0f));
	texUV.push_back(vec2(1.0f, 0.0f));
	
	//v3 -- top right
	vertices.push_back(vec3(size, size, 0.0f));
	texUV.push_back(vec2(1.0f, 1.0f));

	// we'll have two triangles, one being v0,v1,v2 and the other v2,v3,v0
	// triangle 1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	//// triangle 2
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
}
