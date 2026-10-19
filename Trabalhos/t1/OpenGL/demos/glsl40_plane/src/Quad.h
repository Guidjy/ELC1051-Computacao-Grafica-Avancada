#pragma once
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "glslprogram.h"


using namespace glm;


class Quad
{
private:
	GLuint vaoId;
	std::vector<vec2> vertices;
	std::vector<vec3> colors;
	std::vector<GLuint> indices;

	void genQuad(vec3 color);

	void genBuffers();

	void compileAndActivateShaders();

public:
	float size;
	float halfSize;
	vec2 pos;
	mat4 model;

	GLSLProgram shader;

	Quad(float _size, vec2 _pos, vec3 _color);

	void render();
};

