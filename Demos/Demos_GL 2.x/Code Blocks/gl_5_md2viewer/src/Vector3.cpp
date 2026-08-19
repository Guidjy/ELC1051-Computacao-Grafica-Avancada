//----------------------------------------------------------
// Vector3.cpp: implementation of the Vector3 class.
// pozzer@inf.ufsm.br
//----------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Vector3.h"


//----------------------------------------------------------------------
// Construction/Destruction
//----------------------------------------------------------------------

Vector3::Vector3() 
{ 
   x = y = z = 0;    
}

Vector3::~Vector3()
{

}


Vector3::Vector3(double vx, double vy, double vz) 
{
   x = vx;  y = vy;  z = vz; 
}

Vector3::Vector3(const Vector3& u)
{
   //printf("\ncopia");
   x = u.x; 
   y = u.y; 
   z = u.z;
}


void Vector3::set( const float vx, const float vy, const float vz )
{
   x = vx;
   y = vy;
   z = vz;                  
};

void Vector3::set(const Vector3 v)
{
   x = v.x;
   y = v.y;
   z = v.z;                  
};


//----------------------------------------------------------------------
// OPERATORS OPERATORS OPERATORS OPERATORS
//----------------------------------------------------------------------
Vector3& Vector3::operator-= (const Vector3& v)
{
   x -= v.x; y -= v.y; z -= v.z;
   return *this;
}

Vector3& Vector3::operator+= (const Vector3& v)
{
   x += v.x; y += v.y; z += v.z;
   return *this;
}


Vector3& Vector3::operator*= (float f)
{
   x *= f; y *= f; z *= f;
   return *this;
}

Vector3 Vector3::operator + ( const Vector3& v )
{
   Vector3 aux( x + v.x, y + v.y, z + v.z );
   return( aux );
}

Vector3 Vector3::operator / (float f)
{
   Vector3 aux( x/f, y/f, z/f );
   return( aux );
}

//   Subtracao de vetores
Vector3 Vector3::operator - ( const Vector3& v )
{
   Vector3 aux( x - v.x, y - v.y, z - v.z );
   return( aux );
}

//   Produto por escalar (float)
Vector3 Vector3::operator * ( const float s )
{
   Vector3 aux( x * s, y * s, z * s );
   return( aux );
}



void Vector3::normalize()
{
   float norm = (float)sqrt(x*x + y*y + z*z);

   if(norm==0.0)
   {
      printf("\n\nNormalize::Divisao por zero");
      x = 1;
      y = 1;
      z = 1;
      return;
      //exit(1);
   }
   x /= norm;
   y /= norm;
   z /= norm;
}

void Vector3::normalize2D(char *caller)
{
   float norm = (float)sqrt(x*x + z*z);

   if(norm==0.0)
   {
      printf("\n\nNormalize 2D: Divisao por zero: %s", caller);
      x = 1;
      y = 0;
      z = 1;
      print();
      return;
   }
   x /= norm;
   y = 0.0;
   z /= norm;
}


void Vector3::rotatex(float angle)
{
   float c = (float)cos(angle);
   float s = (float)sin(angle);
   float y2 = y*c + z*s;
   float z2 = z*c - y*s;
   y = y2; z = z2;
}

void Vector3::rotatey(float angle)
{
   float c = (float)cos(angle);
   float s = (float)sin(angle);
   float x2 = x*c - z*s;
   float z2 = z*c + x*s;
   x = x2; z = z2;
}

void Vector3::rotatez(float angle)
{
   float c = (float)cos(angle);
   float s = (float)sin(angle);
   float x2 = x*c + y*s;
   float y2 = y*c - x*s;
   x = x2; y = y2;
}

void Vector3::rotate(float rx, float ry, float rz)
{
   rotatex(rx);
   rotatey(ry);
   rotatez(rz);
}



void Vector3::print(void)
{
   printf("\n%.2f %.2f %.2f", x, y, z);
}

void Vector3::print(char *msg)
{
   printf("\n%s = %.2f %.2f %.2f", msg, x, y, z);
}

char* Vector3::toString(void)
{
   char *msg = (char*) malloc(1000);
   sprintf( msg ,"%.1f, %.1f, %.1f", x, y, z);
   return msg;
}
