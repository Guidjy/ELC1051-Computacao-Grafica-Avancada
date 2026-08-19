// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _VECTOR2_H
#define _VECTOR2_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PIOVER180 0.0174532925f
#define PI        3.1415
#define PI2       PI/2

class Vector2  
{
public:
   float x, y;

   Vector2() : x(0.0),y(0.0){}
   Vector2(float a, float b) : x(a), y(b){}
   Vector2(const Vector2& v)
   {
      x = v.x; 
      y = v.y; 
   }
   
   void set(float a, float b)
   {
      x = a;
      y = b;
   }
   
   void set(const Vector2& v)
   {
      x = v.x;
      y = v.y;
   }

   Vector2& operator-= (const Vector2& v)
   {
      x -= v.x; 
      y -= v.y;
      return *this;
   }
   
   Vector2& operator+= (const Vector2& v)
   {
      x += v.x; 
      y += v.y; 
      return *this;
   } 
   
   Vector2& operator*= (const float f)
   {
      x *= f; y *= f;
      return *this;
   }

   Vector2& operator/= (const float f)
   {
      x /= f; y /= f;
      return *this;
   }

   Vector2 operator - ( const Vector2& v )
   {
      Vector2 aux( x - v.x, y - v.y);
      return( aux );
   }

   Vector2 operator + ( const Vector2& v )
   {
      Vector2 aux( x + v.x, y + v.y);
      return( aux );
   }

   Vector2 operator * ( const double s )
   {
      Vector2 aux( x * s, y * s);
      return( aux );
   }

   Vector2 operator / ( const double s )
   {
      Vector2 aux( x / s, y / s);
      return( aux );
   }
   
   float distance(const Vector2& v)
   {
      float dx = x-v.x;
      float dy = y-v.y;
      return sqrt(dx*dx + dy*dy);
   }
   
   Vector2 perp()const
   {
     return Vector2(-y, x);
   }

   float dot(const Vector2& v)
   {
      return x*v.x + y*v.y;
   }
   
   //retorna um angulo entre -PI e PI
   float angle(const Vector2& v)
   {
      float dp, angPI ;
	
      dp = dot( v ); //dot product
      if(dp >= 1.0) dp = 1.0f;
      if(dp <=-1.0) dp =-1.0f;
	
      angPI = (float)acos( dp );

      //determina a posicao relativa do vetor
      return side(v) * angPI;
   }
   
   //retorna a posicao relativa de dois vetores
   int side(const Vector2 &v)
   {
      if (y*v.x > x*v.y)
         return -1;
      else
        return 1;
   }
   
   Vector2 reverse()const
   {
      return Vector2(-this->x, -this->y);
   }
   
   
   void normalize()
   {
      float norm = (float)sqrt(x*x + y*y);

      if(norm==0.0)
      {
         printf("\n\nNormalize::Divisao por zero");
         exit(1);
      }
      x /= norm;
      y /= norm;
   }

   void truncate(float max)
   {
      if( lenghtSqr() > max )
      {
         normalize();
         *this *= max;
      }
   }
   
   void rotate(const float angle)
   {
      float c = (float)cos(angle);
      float s = (float)sin(angle);
      float x2 = x*c - y*s;
      float y2 = y*c + x*s;
      x = x2; 
      y = y2;
   }

   float lenght()
   {
      return (float)(x*x + y*y);
   }

   float   lenghtSqr()
   {
      return (float)sqrt(x*x + y*y);
   }
   float   distance(const Vector2& v) const;

   
   void print(void)
   {
      printf("\n %f %f", x, y);
   }
};

#endif
