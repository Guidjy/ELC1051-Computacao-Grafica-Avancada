#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>

// A list of line segments (pairs of vertices) stored on the GPU.
// Each vertex has a position and a color, kept in two separate VBOs
// and described by one VAO.
class LineMesh
{
public:
	LineMesh() = default;
	~LineMesh();
	LineMesh(const LineMesh&) = delete;
	LineMesh& operator=(const LineMesh&) = delete;

	void clear();
	void addLine(glm::vec3 a, glm::vec3 b, glm::vec3 color);
	// circle of the given radius in the plane spanned by the unit vectors u and v
	void addCircle(glm::vec3 center, glm::vec3 u, glm::vec3 v, float radius, int segments, glm::vec3 color);

	// sends the vertex data to the GPU (call again after changing the lines)
	void upload();
	void draw() const;

private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	GLuint vao = 0;
	GLuint vbo[2] = { 0, 0 };
	GLsizei vertexCount = 0;
};
