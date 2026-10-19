#include "LineMesh.h"
#include <cmath>

LineMesh::~LineMesh()
{
	if (vao != 0)
	{
		glDeleteBuffers(2, vbo);
		glDeleteVertexArrays(1, &vao);
	}
}

void LineMesh::clear()
{
	positions.clear();
	colors.clear();
}

void LineMesh::addLine(glm::vec3 a, glm::vec3 b, glm::vec3 color)
{
	positions.push_back(a);
	positions.push_back(b);
	colors.push_back(color);
	colors.push_back(color);
}

void LineMesh::addCircle(glm::vec3 center, glm::vec3 u, glm::vec3 v, float radius, int segments, glm::vec3 color)
{
	const float twoPi = 6.28318530718f;
	for (int i = 0; i < segments; i++)
	{
		float a0 = twoPi * i / segments;
		float a1 = twoPi * (i + 1) / segments;
		addLine(center + radius * (std::cos(a0) * u + std::sin(a0) * v),
				center + radius * (std::cos(a1) * u + std::sin(a1) * v), color);
	}
}

void LineMesh::upload()
{
	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);  // VAO: remembers which buffers feed which vertex attributes
		glGenBuffers(2, vbo);        // two VBOs: positions and colors

		glBindVertexArray(vao);

		// attribute 0 = position: 3 floats per vertex, tightly packed
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		// attribute 1 = color
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	vertexCount = (GLsizei)positions.size();
	if (vertexCount == 0)
		return;

	// glBufferData (re)allocates GPU memory and copies the CPU data into it.
	// The attribute layout stored in the VAO stays valid.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineMesh::draw() const
{
	if (vertexCount == 0)
		return;
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, vertexCount);  // every 2 vertices form one segment
	glBindVertexArray(0);
}
