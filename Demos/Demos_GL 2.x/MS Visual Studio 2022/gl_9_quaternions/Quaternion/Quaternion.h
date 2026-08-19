// Adaptado de http://www.gamedev.net/reference/articles/article1095.asp

#ifndef __QUATERNION_H_
#define __QUATERNION_H_

#include "Vector.h"

const float PI = 3.14159265358979323846f;

#define	DEGTORAD(x)	( ((x) * PI) / 180.0 )
#define	RADTODEG(x)	( ((x) * 180.0) / PI )
#define LIMIT_RANGE(low, value, high){ if (value < low) value = low; else if(value > high) value = high; }
#define EPSILON	 0.005		// error tolerance for check
#define ZERO_CLAMP(x)	( (EPSILON > fabs(x))?0.0f:(x) ) // set float to 0 if within tolerance
#define FLOAT_EQ(x,v)	( ((v) - EPSILON) < (x) && (x) < ((v) + EPSILON) )		// float equality test

class Quaternion  // a always normalized Quaternion class
{
public:

	float x,y,z,w;		// quaternion representation (w, xi, yj, zk)

	double Norm(void) const;		// get the norm N^2
	
	Quaternion();												// constructor a quaternion with zero rotation
	Quaternion(float x, float y, float z, float w);				// constructs a quaternion from given values. w is in rad, and quat will be normalized. Will perform a valid check
    void eulerToQuat(const float pitch, const float yaw, const float roll);

	void identity(void);														// set this quaternion to the identity quaternion

	void getAxisAngle(Vector3 &v, float &angle) const;	// get an axis angle from this quaternion, angle returned in radians

	void Normalize(void);													// normalize this quaternion

	void EulerToQuat(const float x, const float y, const float z);			// convert euler angles to quaternion, euler in degrees

	void getMatrix(float m[16]) const;	// get the 4x4 rotation matrix representation of this quaternion

	Quaternion	operator*(const Quaternion &q) const;
	Quaternion&	operator=(const Quaternion &q);

	void print(char *);

};

#endif	
