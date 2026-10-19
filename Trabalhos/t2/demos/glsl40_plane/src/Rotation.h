#pragma once
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

// Rotation of a point P by an angle theta around an arbitrary unit axis A
inline glm::mat4 rotateArbitrary(glm::vec3 A, float theta)
{
	A = glm::normalize(A);
	float c = std::cos(theta);
	float s = std::sin(theta);
	float t = 1.0f - c;

	glm::mat4 R(1.0f);
	// GLM is column-major: R[column][row]
	R[0][0] = A.x * A.x * t + c;        R[1][0] = A.x * A.y * t - A.z * s;  R[2][0] = A.x * A.z * t + A.y * s;
	R[0][1] = A.x * A.y * t + A.z * s;  R[1][1] = A.y * A.y * t + c;        R[2][1] = A.y * A.z * t - A.x * s;
	R[0][2] = A.x * A.z * t - A.y * s;  R[1][2] = A.y * A.z * t + A.x * s;  R[2][2] = A.z * A.z * t + c;
	return R;
}

// Applies a rotation matrix to a direction (w = 0, so translations are ignored)
inline glm::vec3 rotateDir(const glm::mat4& R, glm::vec3 v)
{
	return glm::vec3(R * glm::vec4(v, 0.0f));
}
