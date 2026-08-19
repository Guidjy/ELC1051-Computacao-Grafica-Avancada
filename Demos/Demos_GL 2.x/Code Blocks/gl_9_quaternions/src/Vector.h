#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <math.h>
#include <stdio.h>

class Vector3  
{
public:
   float x, y, z;
   Vector3() 
   {
      x = y = z = 0; 
   }
   Vector3(float vx, float vy, float vz) 
   {
      x = vx;  y = vy;  z = vz; 
   }

   Vector3 normalize()
   {
   	  float length = sqrt( x*x + y*y + z*z );
   	  if( length > 0.000001)
   	  {
         x/= length;
         y/= length;
         z/= length;
      }
      else
         printf("\nError");
   }

   void set(float vx, float vy, float vz) 
   {
      x = vx;  y = vy;  z = vz; 
   }
  
   void print()
   {
      printf("\n%.2f %.2f %.2f ", x, y, z);     
   }
};

#endif
