#include "Vector.h"
#include <math.h>
#include <stdio.h>

Vector::Vector()
{
   x = y = 0;             
}

Vector::Vector(float _x, float _y)
{
   x = _x;
   y = _y;
}

void Vector::set(float _x, float _y)
{
   x = _x;
   y = _y;
}

//retorna entre [-1, 1] pois senao da erro no calculo do acos(ang)
float Vector::dot(const Vector& v) const
{
   float ret;
   ret = x*v.x + y*v.y;
   return ret;
}

float Vector::angleSign(const Vector& v)const 
{
   float dp, angPi ;
	
   dp = dot( v ); //dot product
   if(dp >= 1.0) dp = 1.0f;
   if(dp <=-1.0) dp =-1.0f;
	
   angPi = (float)acos( dp );

   //teste de lado
   if (y*v.x > x*v.y)
      return -angPi;
   else 
      return angPi;
}

void Vector::normalize()
{
   float norm = (float)sqrt(x*x + y*y);

   if(norm==0.0)
   {
      printf("\n\nNormalize::Divisao por zero");
      x = 1;
      y = 1;
      return;
   }
   x /= norm;
   y /= norm;
}

void Vector::print()
{
   printf("\nVector: %.3f %.3f", x, y);
}

Vector Vector::operator - ( const Vector& v )
{
   Vector aux( x - v.x, y - v.y);
   return( aux );
}

Vector Vector::operator + ( const Vector& v )
{
   Vector aux( x + v.x, y + v.y);
   return( aux );
}

//	Produto por escalar (float)
Vector Vector::operator * ( const float s )
{
   Vector aux( x * s, y * s );
   return( aux );
}

float Vector::norm() const
{
   return (float)sqrt(x*x + y*y);
}
