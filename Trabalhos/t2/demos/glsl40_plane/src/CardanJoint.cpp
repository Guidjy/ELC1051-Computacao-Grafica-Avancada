#include "CardanJoint.h"
#include "Rotation.h"
#include <cmath>

using glm::vec3;
using glm::mat4;

static const vec3 X_AXIS(1.0f, 0.0f, 0.0f);
static const vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
static const vec3 Z_AXIS(0.0f, 0.0f, 1.0f);
static const float PI = 3.14159265359f;

// Rotation that tilts the X axis onto the secondary shaft direction: a rotation of beta
// around Z (the axis perpendicular to both shafts), so X goes to (cos beta, sin beta, 0).
static mat4 bendRotation(float beta)
{
	return rotateArbitrary(Z_AXIS, beta);
}

vec3 CardanJoint::outputAxis() const
{
	return rotateDir(bendRotation(beta), X_AXIS);
}

JointPose CardanJoint::solve(float theta1) const
{
	JointPose pose;

	// Input shaft: rotation of theta1 around its own axis (X)
	mat4 Rin = rotateArbitrary(X_AXIS, theta1);
	pose.inputModel = Rin;

	// The input yoke holds the cross arm a. Before any tilt, the other arm b0 is just Z rotated with the shaft.
	vec3 a = rotateDir(Rin, Y_AXIS);
	vec3 b0 = rotateDir(Rin, Z_AXIS);

	// Crosspiece: it may only pivot around arm a (the input yoke pins).
	// Turning b0 by gamma around a gives b = b0 cos(gamma) + (a x b0) sin(gamma), and a x b0 = X.
	// Arm b must sit in the output yoke, i.e. be perpendicular to the output shaft d2:
	//   b . d2 = cos(gamma)(b0 . d2) + sin(gamma)(X . d2) = 0  ->  tan(gamma) = -(b0 . d2) / (X . d2)
	vec3 d2 = outputAxis();
	float gamma = std::atan2(-glm::dot(b0, d2), glm::dot(X_AXIS, d2));
	mat4 Rcross = rotateArbitrary(a, gamma) * Rin;
	pose.crossModel = Rcross;
	vec3 b = rotateDir(Rcross, Z_AXIS);

	// Output shaft: first tilted (bend), then spun by theta2 around its own axis d2.
	// Its yoke pins start along z0 (Z after the tilt) and have to end up along arm b,
	// so theta2 is the signed angle from z0 to b measured around d2.
	mat4 Rbend = bendRotation(beta);
	vec3 z0 = rotateDir(Rbend, Z_AXIS);
	pose.theta2 = std::atan2(glm::dot(glm::cross(z0, b), d2), glm::dot(z0, b));
	pose.outputModel = rotateArbitrary(d2, pose.theta2) * Rbend;

	return pose;
}

float CardanJoint::speedRatio(float theta1) const
{
	// Central difference of theta2 with respect to theta1. Since the input spins at a
	// constant speed w1, the output speed is w2 = w1 * d(theta2)/d(theta1).
	const float h = 5e-3f;
	float d = solve(theta1 + h).theta2 - solve(theta1 - h).theta2;
	if (d > PI) d -= 2.0f * PI;   // theta2 wraps around at +-pi
	if (d < -PI) d += 2.0f * PI;
	return d / (2.0f * h);
}
