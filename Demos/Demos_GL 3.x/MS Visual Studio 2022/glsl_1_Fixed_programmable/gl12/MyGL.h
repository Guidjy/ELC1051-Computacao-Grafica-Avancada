//---------------------------------------------
// Author: Cesar Tadeu Pozzer
// UFSM - 06/2010
// http://www-usr.inf.ufsm.br/~pozzer/
//
// - This class implements functions prunned from OpenGL 3.2 API
// - They use the same OpenGL 1.0 sintax for
//     * matrices
//     * modelview functions
//     * projection functions
//
// Version 1.1
//---------------------------------------------
//---------------------------------------------

#include <GL/glew.h>

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define PIf               3.1415926535897932384626433832795f
#define MATRIX_STACK_SIZE 10 //maximum number of push operations allowed
#define MATRIX_SIZE       16 //4X4

//constants already defined in OpenGL 1.0
//#define GL_MODELVIEW  1
//#define GL_PROJECTION 2
//#define GL_TEXTURE    3

class MyGL
{
private:
   int m_idx_modelViewMatrix; //top of the matrix
   int m_idx_projectionMatrix;
   
   int m_currentMatrixType; //the current matrix: ModelView or Projection

   GLfloat m_projectionMatrix[MATRIX_SIZE*MATRIX_STACK_SIZE];
   GLfloat m_modelViewMatrix[MATRIX_SIZE*MATRIX_STACK_SIZE];
   //GLfloat m_textureMatrix[MATRIX_SIZE*MATRIX_STACK_SIZE];
   //GLfloat m_normalMatrix[MATRIX_SIZE*MATRIX_STACK_SIZE];

   GLfloat *m_currentMatrix;  //pointer to the current matrix

public: 
   MyGL();

   void popMatrix();  //pop the current matrix
   void pushMatrix(); //push the current matrix.
   void matrixMode(int type); //GL_MODELVIEW or GL_PROJECTION 

//functions to access matrices
   GLfloat * getProjectionMatrix();
   GLfloat * getModelViewMatrix();

//transform funcions 
   void translate( float x, float y, float z);
   void rotate( float angle, float x, float y, float z);
   void scale( float x, float y, float z);
   void multMatrix(const float matrix[16]);
   void loadIdentity();
   void printMatrix();

//camera and projection funcions 
   void perspective( float fovy, float aspect, float zNear,  float zFar);
   void lookAt(      float eyeX, float eyeY,   float eyeZ,   float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
   void frustum(     float left, float right,  float bottom, float top, float nearVal, float farVal);
   void ortho(       float left, float right,  float bottom, float top, float nearVal, float farVal);
   
//vector functions
   inline void  normalize(float vector[3]);
   inline float dot(const float vector0[3], const float vector1[3]);
   inline void  cross(float result[3], const float vector0[3], const float vector1[3]);
   inline float amount(const float vector[3]);


private:
   void loadIdentity(float matrix[16]);
};