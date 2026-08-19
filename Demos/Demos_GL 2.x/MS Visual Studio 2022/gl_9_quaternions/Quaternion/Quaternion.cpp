// Adaptado de http://www.gamedev.net/reference/articles/article1095.asp

#include "Quaternion.h"
#include <math.h>

Quaternion::Quaternion()  //unit quaternion
{
   identity();
}

Quaternion::Quaternion(const float x, const float y, const float z, const float w)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->w = w;

   //assert(!FLOAT_EQ(0.0f, Norm()));		// norm should never be close to 0
   Normalize();
}	

void Quaternion::identity()
{
   x = 0.0f;
   y = 0.0f;
   z = 0.0f;
   w = 1.0f;
}

void Quaternion::eulerToQuat(const float pitch, const float yaw, const float roll)
{
	double	ex, ey, ez;		// temp half euler angles
	double	cos_r, cos_p, cos_y; //roll, pitch, yaw
    double  sin_r, sin_p, sin_y;
    double  cos_y_cos_r, sin_y_sin_r;

	ex = DEGTORAD(pitch) / 2.0;	// convert to rads and half them
	ey = DEGTORAD(yaw) / 2.0;
	ez = DEGTORAD(roll) / 2.0;

	cos_p = cos(ex);  //antes: cos_r, cos_p, cos_y
	cos_y = cos(ey);
	cos_r = cos(ez);

	sin_p = sin(ex);
	sin_y = sin(ey);
	sin_r = sin(ez);

    //Solucao 1 - otimizada 
    //--------------------------------------------
	/*
    cos_y_cos_r = cos_y * cos_r;
	sin_y_sin_r = sin_y * sin_r;
	this->w = float(cos_p * cos_y_cos_r + sin_p * sin_y_sin_r);
	this->x = float(sin_p * cos_y_cos_r - cos_p * sin_y_sin_r);
	this->y = float(cos_p * sin_y * cos_r + sin_p * cos_y * sin_r);
	this->z = float(cos_p * cos_y * sin_r - sin_p * sin_y * cos_r);
    /**/
	
	//Solucao 2 - passo a passo
    //--------------------------------------------
	Quaternion qx(sin_p,0,0,  cos_p);
	Quaternion qy(0,sin_y,0,  cos_y);
	Quaternion qz(0,0,sin_r,  cos_r);
	Quaternion q;
	q = qx * qy * qz;
	this->x = q.x;
	this->y = q.y;
	this->z = q.z;
	this->w = q.w;
    /**/
	Normalize();
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
	double rx, ry, rz, rw;		// temp result

	rx	= q.w*x + q.x*w + q.y*z - q.z*y;
	ry	= q.w*y + q.y*w + q.z*x - q.x*z;
	rz	= q.w*z + q.z*w + q.x*y - q.y*x;
	rw	= q.w*w - q.x*x - q.y*y - q.z*z;

	return(Quaternion((float)rx, (float)ry, (float)rz, (float)rw));
}

void Quaternion::print(char *s)
{
   printf("\nQuaternion %s = (%.6f %.6f %.6f)  %.12f", s, x, y, z, w);
}



double Quaternion::Norm(void) const
{
   return( sqrt( x*x + y*y + z*z + w*w) );
}	

void Quaternion::Normalize(void)
{
	double norm = Norm();

	//assert(!FLOAT_EQ(0.0f, norm));		// norm should never be close to 0

	x = float(x / norm);
	y = float(y / norm);
	z = float(z / norm);
	w = float(w / norm);

	//assert(FLOAT_EQ(1.0f, Norm()));		// must be normalized, safe

	LIMIT_RANGE(-1.0f, w, 1.0f);

	LIMIT_RANGE(-1.0f, x, 1.0f);
	LIMIT_RANGE(-1.0f, y, 1.0f);
	LIMIT_RANGE(-1.0f, z, 1.0f);
}

void Quaternion::getAxisAngle(Vector3 &v, float &angle) const
{
	double	temp_angle;		// temp angle
	double	scale;			// temp vars

	temp_angle = acos(w);

	scale = (float)sqrt(x*x + y*y + z*z);

	//assert(0 <= temp_angle);		// make sure angle is 0 - PI
	//assert(PI >= temp_angle);

	if (FLOAT_EQ(0.0f, scale))		// angle is 0 or 360 so just simply set axis to 0,0,1 with angle 0
	{
		angle = 0.0f;
        v.x = v.y =v.z =0.0;  
	}
	else
	{
		angle = (float)(temp_angle * 2.0);		// angle in radians

		v.x = x/scale;
		v.y = y/scale;
		v.z = z/scale;
		v.normalize();

		//assert(0.0f <= angle);			// make sure rotation around axis is 0 - 360
		//assert(2*PI >= angle);
		//assert(v.IsUnit());				// make sure a unit axis comes up
	}
}	

void Quaternion::getMatrix(float matrix[16]) const
{
	double x2, y2, z2, w2, xy, xz, yz, wx, wy, wz;

	//float matrix[16];		// temp matrix

	x2	= x*x; 	
    y2	= y*y;	
    z2	= z*z;
    w2	= w*w;

	xy	= x * y;
	xz	= x * z;
	yz	= y * z;
	wx	= w * x;
	wy	= w * y;
	wz	= w * z;

	// should be slightly more accurate than above due to rearranging for unit quaternions only
	matrix[0] =	float(1 - 2*(y2 + z2));
	matrix[1] =	float(2 * (xy + wz));
	matrix[2] =	float(2 * (xz - wy));
	matrix[3] = 0;

	matrix[4] =	float(2 * (xy - wz));
	matrix[5] =	float(1 - 2*(x2 + z2));
	matrix[6] =	float(2 * (yz + wx));
	matrix[7] = 0;

	matrix[8] =	float(2 * (xz + wy));
	matrix[9] =	float(2 * (yz - wx));
	matrix[10]= float(1 - 2*(x2 + y2));
	matrix[11]= 0;
	
	matrix[12]= 0;
	matrix[13]= 0;
	matrix[14]= 0;
	matrix[15]= 1;
}	

Quaternion& Quaternion::operator=(const Quaternion &q)
{
   x = q.x;
   y = q.y;
   z = q.z;
   w = q.w;
   return(*this);
}

/*void Quaternion::Slerp(const Quaternion q1, const Quaternion q2, const float slerp)
{
   double	cosom, omega, sinom, scale1, scale2;
   float tx, ty, tz, tw;			// temp vars

   cosom	= q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w; //dot product
   omega	= acos(cosom);		// use SLERP
   sinom	= sin(omega);

			scale1	= sin((1.0f - slerp) * omega) / sinom;
			scale2	= sin(slerp * omega) / sinom;
		}

		tx	= q2.x;
		ty	= q2.y;
		tz	= q2.z;
		tw	= q2.w;



	x	= float(scale1*q1.x + scale2*q2.x);
	y	= float(scale1*q1.y + scale2*q2.y);
	z	= float(scale1*q1.z + scale2*q2.z);
	w	= float(scale1*q1.w + scale2*q2.w);

	Normalize();
}*/

