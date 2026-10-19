#include "Quad.h"
#include <array>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


Quad::Quad(float _size, vec2 _pos, vec3 _color)
{
	size = _size;
	halfSize = size / 2;
	pos = _pos;

	mat4 identity = mat4(1.0f);
	model = translate(identity, vec3(pos, 0.0f));

	genQuad(_color);
	genBuffers();
	compileAndActivateShaders();
}

void Quad::genQuad(vec3 color)
{
	// bottom-left vertex
	vertices.push_back(vec2(-halfSize, -halfSize));
	colors.push_back(color);

	// bottom-right vertex
	vertices.push_back(vec2(halfSize, -halfSize));
	colors.push_back(color);

	// upper-left vertex
	vertices.push_back(vec2(-halfSize, halfSize));
	colors.push_back(color);

	// upper-right vertex
	vertices.push_back(vec2(halfSize, halfSize));
	colors.push_back(color);

	// triangle 1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// triangle 2
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
}

void Quad::genBuffers()
{
	glGenVertexArrays(1, &vaoId);  // generates and reserves a unique ID for one VAO
	glBindVertexArray(vaoId);	   // makes the VAO assigned to the vaoId the currently active vertex array in the state machine

	auto handles = std::array<GLuint, 3>();  // buffer handles (buffer IDs)
	glGenBuffers(3, handles.data());		 // creates 3 sepparate VBO's

	// vertex position VBO
	glBindBuffer(GL_ARRAY_BUFFER, handles[0]);
	// copies data in vertices to GPU mem
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	// defines how the GPU interprets the bytes in handles[0]. 0 = vertex attribute 0 in the vertex shader
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// enables vertex attribute location 0 within the active VAO so the vertex shader can consume it during rendering
	glEnableVertexAttribArray(0);

	// vertex color VBO
	glBindBuffer(GL_ARRAY_BUFFER, handles[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), (GLvoid*)&colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);

	// index buffer (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	// unbinds the current VAO
	glBindVertexArray(0);
}

void Quad::compileAndActivateShaders()
{
	try {
		shader.compileShader("shader/glsl40_plane.vert");
		shader.compileShader("shader/glsl40_plane.frag");

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException& e) {
		std::cerr << e.what() << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();
}

void Quad::render()
{
	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
	glBindVertexArray(0);
}