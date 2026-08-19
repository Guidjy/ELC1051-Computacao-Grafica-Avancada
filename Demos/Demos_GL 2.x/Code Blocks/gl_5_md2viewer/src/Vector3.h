// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _VECTOR3___H
#define _VECTOR3___H


#define PIOVER180 0.0174532925f
#define PI        3.1415
#define PI2       PI/2

class Vector3
{
public:

   double x, y, z;
   virtual ~Vector3();

   Vector3();
   Vector3(double vx, double vy, double vz);
   Vector3(const Vector3& u);
   void set(const float vx, const float vy, const float vz);
   void set(Vector3 v);

   Vector3& operator-= (const Vector3& v2);
   Vector3& operator+= (const Vector3& v2);
   Vector3& operator*= (float f);
   Vector3  operator / (float f);
   Vector3  operator + (const Vector3& v);
   Vector3  operator - (const Vector3& v);
   Vector3  operator * (const float s);

   void normalize();
   void normalize2D(char *caller);
   void rotatex(float angle);
   void rotatey(float angle);
   void rotatez(float angle);
   void rotate(float rx, float ry, float rz);

   void print(void);
   void print(char *msg);

   char* toString(void);
};

#endif
