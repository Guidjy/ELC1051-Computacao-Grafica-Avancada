#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


//Cardan (universal) joint centred at the origin.
// - Primary (input) shaft: lies on the X axis and spins around it by theta1.
// - Secondary (output) shaft: tilted from the X axis by the bend angle beta (-45..45 deg)
//   inside the XY plane (positive beta tilts it up, negative beta tilts it down).
// - Crosspiece: one arm (a) is held by the input yoke, the other arm (b) by the output
//  yoke. Both arms are perpendicular to each other, a is perpendicular to the input
//  shaft and b is perpendicular to the output shaft. These constraints give theta2.
struct JointPose
{
	glm::mat4 inputModel;   // input shaft + yoke
	glm::mat4 crossModel;   // crosspiece
	glm::mat4 outputModel;  // output shaft + yoke
	float theta2;           // output shaft angle around its own axis (radians, -pi..pi)
};

class CardanJoint
{
public:
	float beta = 0.0f;  // signed angle between the shafts (radians)

	glm::vec3 outputAxis() const;          // unit direction of the secondary shaft
	JointPose solve(float theta1) const;   // pose of all the parts for an input angle

	// d(theta2)/d(theta1): output speed divided by the input speed
	float speedRatio(float theta1) const;
};
