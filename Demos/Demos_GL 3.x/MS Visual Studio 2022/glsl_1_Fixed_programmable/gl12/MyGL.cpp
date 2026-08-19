
//---------------------------------------------
// Author: Cesar Tadeu Pozzer
// UFSM - 06/2010
// Most functions are adapted from Norbert Nopper  - 
//           norbert@nopper.tv
//           http://nopper.tv
//           http://nopper.tv/source/OpenGL_3_2.zip
// Math code also available at
//           http://www.opengl.org/sdk/docs/man/xhtml/
//
// - This class implements functions prunned from OpenGL 3.2 API
// - They use the same OpenGL 1.0 sintax for
//     * matrices
//     * modelview functions
//     * projection functions
//
// Version 1.1
//

#include <GL/glew.h>

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MyGL.h" 


//Pozzer
//=======================================
MyGL::MyGL()
{
   m_idx_modelViewMatrix  = 0;
   m_idx_projectionMatrix = 0;
   m_currentMatrix = m_modelViewMatrix; //default matrix 
   m_currentMatrixType  = GL_MODELVIEW;
   
   //initialize the bottom of matrices
   loadIdentity(m_modelViewMatrix);
   loadIdentity(m_projectionMatrix);
}

//Pozzer
//=======================================
void MyGL::printMatrix()
{
   printf("\n");
   for (int i = 0; i < 16; i++)
   {
	   if(i%4==0)
         printf("\n");
	   printf("%.2f  ", m_currentMatrix[i]);
   }
}

//Pozzer
//=======================================
void MyGL::matrixMode(int type) // GL_MODELVIEW or GL_PROJECTION 
{
   if( type == GL_MODELVIEW ) 
 	  m_currentMatrix = m_modelViewMatrix + (m_idx_modelViewMatrix*MATRIX_SIZE);
   else if (type == GL_PROJECTION )
 	  m_currentMatrix = m_projectionMatrix + (m_idx_projectionMatrix*MATRIX_SIZE);
   else
   {
	  printf("\nError: matrixMode - Invalid argument: %d", type);
	  return;
   }
   m_currentMatrixType = type;
}


//Pozzer
//=======================================
void MyGL::pushMatrix()
{
   unsigned int i;

   if( m_currentMatrixType == GL_MODELVIEW ) 
   {
      if( m_idx_modelViewMatrix >= (MATRIX_STACK_SIZE - 2) )	
	  {
		  printf("\n Error: Modelview Matrix Stack is Full");
		  getchar();
		  exit(0);
	  }
      m_idx_modelViewMatrix ++;
	  m_currentMatrix = m_modelViewMatrix + (m_idx_modelViewMatrix * MATRIX_SIZE);
   }
   else if (m_currentMatrixType == GL_PROJECTION )
   {
	  if( m_idx_projectionMatrix >= (MATRIX_STACK_SIZE - 2) )	
	  {
		  printf("\n Error: Projection Matrix Stack is Full");
		  getchar();
		  exit(0);
	  }
      m_idx_projectionMatrix ++;
	  m_currentMatrix = m_projectionMatrix + (m_idx_projectionMatrix * MATRIX_SIZE);
   }

   for (i = 0; i < MATRIX_SIZE; i++)
   {
      m_currentMatrix[i+MATRIX_SIZE] = m_currentMatrix[i];
   }
}

//Pozzer
//=======================================
void MyGL::popMatrix()
{
   if( m_currentMatrixType == GL_PROJECTION ) 
   {
	  if( m_idx_projectionMatrix > 0)
		  m_idx_projectionMatrix --;
	  m_currentMatrix = m_projectionMatrix + (m_idx_projectionMatrix * MATRIX_SIZE);
   }
   else if (m_currentMatrixType == GL_MODELVIEW )
   {
 	  if( m_idx_modelViewMatrix > 0)
		  m_idx_modelViewMatrix --;
	  m_currentMatrix = m_modelViewMatrix + (m_idx_modelViewMatrix * MATRIX_SIZE);
   }
}

//Pozzer
//=======================================
GLfloat * MyGL::getProjectionMatrix()
{
   return m_projectionMatrix + (m_idx_projectionMatrix * MATRIX_SIZE);
}
 
//Pozzer
//=======================================
GLfloat * MyGL::getModelViewMatrix()
{
   return m_modelViewMatrix + (m_idx_modelViewMatrix * MATRIX_SIZE);
}



//=================================================================================
//Functions based on http://nopper.tv/source/OpenGL_3_2.zip
//=================================================================================
void MyGL::multMatrix(const float matrix[16])
{
	unsigned int i;
	unsigned int k; 

	float temp[16];

	for (i = 0; i < 16; i++)
	{
		temp[i] = 0.0f;
		
		for(k = 0; k < 4; k++)
		{
			temp[i] += m_currentMatrix[(i%4)+(k*4)] * matrix[(k)+((i/4)*4)];
		}
	}
		
	for (i = 0; i < 16; i++)
	{
		m_currentMatrix[i] = temp[i];
	}
}

void MyGL::translate( float x, float y, float z)
{
	float temp[16];
	loadIdentity(temp);

	temp[12] = x;
	temp[13] = y;
	temp[14] = z;
	
	multMatrix(temp);
}

//http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
void MyGL::rotate( float angle, float x, float y, float z)
{
	float temp[16];

	//pozzer
	//------------------------
	float vet[3] = {x, y, z};
	normalize(vet);
	x = vet[0];
	y = vet[1];
	z = vet[2];

	float s = sinf(2.0f*PIf*angle/360.0f);
	float c = cosf(2.0f*PIf*angle/360.0f);

	loadIdentity(temp);

	temp[0] = x*x*(1 - c) + c;
	temp[1] = x*y*(1 - c) + z*s;
	temp[2] = x*z*(1 - c) - y*s;

	temp[4] = x*y*(1 - c) - z*s;
	temp[5] = y*y*(1 - c) + c;
	temp[6] = y*z*(1 - c) + x*s;

	temp[8] = x*z*(1 - c) + y*s;
	temp[9] = y*z*(1 - c) - x*s;
	temp[10] = z*z*(1 - c) + c;

	multMatrix(temp);
}

void MyGL::scale( float x, float y, float z)
{
	float temp[16];

	loadIdentity(temp);

	temp[0] = x;
	temp[5] = y;
	temp[10] = z;
	
	multMatrix(temp);
}

//http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
void MyGL::perspective( float fovy, float aspect, float zNear, float zFar)
{
   float xmin, xmax, ymin, ymax;

   ymax = zNear * tanf(fovy * PIf / 360.0f);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;

   frustum( xmin, xmax, ymin, ymax, zNear, zFar);
}

//http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
void MyGL::lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
   float forward[3], side[3], up[3];
   float matrix[16];

   forward[0] = centerX - eyeX;
   forward[1] = centerY - eyeY;
   forward[2] = centerZ - eyeZ;
   normalize(forward);

   up[0] = upX;
   up[1] = upY;
   up[2] = upZ;
    
   cross(side, forward, up);
   normalize(side);

   cross(up, side, forward);

   matrix[0] = side[0];
   matrix[1] = up[0];
   matrix[2] = -forward[0];
   matrix[3] = 0.0f;
   matrix[4] = side[1];
   matrix[5] = up[1];
   matrix[6] = -forward[1];
   matrix[7] = 0.0f;
   matrix[8] = side[2];
   matrix[9] = up[2];
   matrix[10] = -forward[2];
   matrix[11] = 0.0f;
   matrix[12] = 0.0f;
   matrix[13] = 0.0f;
   matrix[14] = 0.0f;
   matrix[15] = 1.0f;

   loadIdentity(m_currentMatrix);
   multMatrix( matrix);
   translate(-eyeX, -eyeY, -eyeZ);
}

//http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml
void MyGL::frustum( float left, float right, float bottom, float top, float nearVal, float farVal)
{
	m_currentMatrix[0] = 2.0f*nearVal/(right-left);
	m_currentMatrix[1] = 0.0f;
	m_currentMatrix[2] = 0.0f;
	m_currentMatrix[3] = 0.0f;
	m_currentMatrix[4] = 0.0f;
	m_currentMatrix[5] = 2.0f*nearVal/(top-bottom);
	m_currentMatrix[6] = 0.0f;
	m_currentMatrix[7] = 0.0f;
	m_currentMatrix[8] = (right+left)/(right-left);
	m_currentMatrix[9] = (top+bottom)/(top-bottom);
	m_currentMatrix[10] = -(farVal+nearVal)/(farVal-nearVal);
	m_currentMatrix[11] = -1.0f;
	m_currentMatrix[12] = 0.0f;
	m_currentMatrix[13] = 0.0f;
	m_currentMatrix[14] = -(2.0f*farVal*nearVal)/(farVal-nearVal);
	m_currentMatrix[15] = 0.0f;
}

//http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
void MyGL::ortho( float left, float right, float bottom, float top, float nearVal, float farVal)
{
	m_currentMatrix[0] = 2.0f/(right-left);
	m_currentMatrix[1] = 0.0f;
	m_currentMatrix[2] = 0.0f;
	m_currentMatrix[3] = 0.0f;
	m_currentMatrix[4] = 0.0f;
	m_currentMatrix[5] = 2.0f/(top-bottom);
	m_currentMatrix[6] = 0.0f;
	m_currentMatrix[7] = 0.0f;
	m_currentMatrix[8] = 0.0f;
	m_currentMatrix[9] = 0.0f;
	m_currentMatrix[10] = -2.0f/(farVal-nearVal);
	m_currentMatrix[11] = 0.0f;
	m_currentMatrix[12] = -(right+left)/(right-left);
	m_currentMatrix[13] = -(top+bottom)/(top-bottom);
	m_currentMatrix[14] = -(farVal+nearVal)/(farVal-nearVal);
	m_currentMatrix[15] = 1.0f;
}

void MyGL::normalize(float vector[3])
{
	unsigned int i;

	float a = amount(vector);

    if (a == 0.0f)
	{
    	return;
	}

	for (i = 0; i < 3; i++)
	{
    	vector[i] /= a;
	}
}

float MyGL::dot(const float vector0[3], const float vector1[3])
{
	return vector0[0]*vector1[0] + vector0[1]*vector1[1] + vector0[2]*vector1[2];
}

void MyGL::cross(float result[3], const float vector0[3], const float vector1[3])
{
	unsigned int i;

	float	temp[3];
	
	temp[0] = vector0[1]*vector1[2] - vector0[2]*vector1[1];
	temp[1] = vector0[2]*vector1[0] - vector0[0]*vector1[2];
	temp[2] = vector0[0]*vector1[1] - vector0[1]*vector1[0];
	
	for (i = 0; i < 3; i++)
	{
    	result[i] = temp[i];
	}
}

float MyGL::amount(const float vector[3])
{
	return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void MyGL::loadIdentity()
{
   loadIdentity(m_currentMatrix);
}

void MyGL::loadIdentity(float *matrix)
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

