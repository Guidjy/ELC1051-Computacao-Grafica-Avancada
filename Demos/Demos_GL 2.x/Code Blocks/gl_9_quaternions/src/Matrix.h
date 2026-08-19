// Adaptado de http://www.gamedev.net/reference/articles/article1095.asp

#ifndef ___MATRIX_H_
#define ___MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

class Matrix
{
public:
   float m[16];
   Matrix()
   {
      identity();
   }

   void identity()
   {
      memset(m, 0, 16*sizeof(float));		// set all to 0
      m[0]	= 1;		// set diagonal to 1
      m[5]	= 1;
      m[10]	= 1;
      m[15]	= 1;
   }

   void rotacionaEixo(Vector3 axis, float angle)
   {
      float x = axis.x;
      float y = axis.y;
      float z = axis.z;
      float c = cosf(DEGTORAD(angle));
      float s = sinf(DEGTORAD(angle));

      //axis.print();

      m[0] = (x * x) * (1.0f - c) + c;
      m[1] = (x * y) * (1.0f - c) + (z * s);
      m[2] = (x * z) * (1.0f - c) - (y * s);
      m[3] = 0.0f;

      m[4] = (y * x) * (1.0f - c) - (z * s);
      m[5] = (y * y) * (1.0f - c) + c;
      m[6] = (y * z) * (1.0f - c) + (x * s);
      m[7] = 0.0f;

      m[8] = (z * x) * (1.0f - c) + (y * s);
      m[9] = (z * y) * (1.0f - c) - (x * s);
      m[10] = (z * z) * (1.0f - c) + c;
      m[11] = 0.0f;

      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;
   }

   float *getMatrix()
   {
      return m;
   }

   Matrix operator*(const Matrix &m) const
   {
      Matrix result;		// temp vars
      double sum;
      int    index, alpha, beta;		// loop vars

      for (index = 0; index < 4; index++)			// perform multiplcation the slow and safe way
      {
         for (alpha = 0; alpha < 4; alpha++)
         {
			sum = 0.0f;
            for (beta = 0; beta < 4; beta++)
               sum += this->m[index + beta*4] * m.m[alpha*4 + beta];
            result.m[index + alpha*4] = (float)sum;
	     }
      }
      return( result );
   }
};

#endif
