// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _VECTOR3_H
#define _VECTOR3_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define PIOVER180 0.0174532925f

class Vector3  
{
public:

	float x, y, z;

	Vector3() { x = y = z = 0; }
	Vector3(float vx, float vy, float vz) {x = vx;  y = vy;  z = vz; }
	Vector3(const Vector3& u);
	virtual ~Vector3();
	void set(float vx, float vy, float vz);
	void set(Vector3 v);

	Vector3& operator-= (const Vector3& v2);
	Vector3& operator+= (const Vector3& v2);
	Vector3& operator*= (float f);
	Vector3  operator ^ (Vector3& v);
    Vector3  operator + (const Vector3& v);
    Vector3  operator - (Vector3& v);
    Vector3  operator * (const float s);

	Vector3 cross(Vector3& u);
	float   dot(Vector3& v);
	float   dot2D(Vector3& v);
	float   angulo(Vector3& v);
	float   angulo2D(Vector3& v);
	float   modulo();
	void    normalize();
	void    normalize2D();
	void    inverte();
	void    perpendicular();
	void  rotatex(float angle);
	void  rotatey(float angle);
	void  rotatez(float angle);
	void  rotate(float rx, float ry, float rz);
	float distancia3D(Vector3& v, bool sqr); //bool indica se deve ser calculada a raiz
	float distancia2D(Vector3& v, bool sqr); //bool indica se deve ser calculada a raiz
	float distanciaReta(Vector3& p1, Vector3& p2); //distancia a uma reta R=(p1, p2)
	bool  interRetaCirc(Vector3& p1, Vector3& p2, float raio);

	void print(void);
};

#endif
