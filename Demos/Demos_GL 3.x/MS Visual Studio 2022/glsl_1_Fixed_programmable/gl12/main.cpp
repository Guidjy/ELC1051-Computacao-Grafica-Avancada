
//http://www.packtpub.com/article/opengl-glsl-4-shaders-basics


/**
*      OpenGL 1.0 vs OpenGL 2.0 vs OpenGL 3.2
*      ----------------------------------------------------------------------------
*   Demo to ilustrate:
*   - the use of vertex and fragment shaders
*   - Comparison between fixed (OpenGL 1.0) e programmable (OpenGL 2.0 vs 3.2) pipelines
*   - a class to handle OpenGL commands on CPU
*   - class to handle shader functions

*   Author: Cesar Tadeu Pozzer
*   UFSM - 06/2010
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*   http://www-usr.inf.ufsm.br/~pozzer/
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

#include "glsl.h"
#include "MyGL.h"

//uniform variables
GLint loc_u_projection;
GLint loc_u_modelview;

Glsl *shader1 = NULL, *shader2 = NULL;
MyGL *gl = NULL;

float ang = 0;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   gl->matrixMode(GL_MODELVIEW);
   glMatrixMode(GL_MODELVIEW);


   //111111111111111111111111111111111111111111111111111111111111111
   //------------------------------------------------------------------------
   //without any shader and with modelview/projection GL Commands
   //------------------------------------------------------------------------
   glColor3f(0,0,1);
   glLoadIdentity();
   gluLookAt( 0,10,15, 0,0,0,  0,1,0);
   glTranslated(-4,4,0);
   glRotated( ang, 0, 1, 1);
   glScalef( 3,3,3);
   glutSolidIcosahedron();



   ///222222222222222222222222222222222222222222222222222222222222222
   //------------------------------------------------------------------------
   //without any shader and without modelview/projection GL Commands
   //------------------------------------------------------------------------
   glColor3f(0,1,1);
   gl->loadIdentity();
   gl->lookAt(0,10,15, 0,0,0,  0,1,0);
   gl->translate(4,4,0);
   gl->rotate(ang, 0, 1, 1);
   gl->scale(3,3,3);
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(gl->getProjectionMatrix());
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(gl->getModelViewMatrix());
   glutSolidIcosahedron();



   //33333333333333333333333333333333333333333333333333333333333333333333333
   //------------------------------------------------------------------------
   //uses shader and user-defined matrices to specify camera, viewing and projection.
   //It doesn't use VBO to specify vertices primitives
   //===================================================================
   shader1->setActive(true);
   gl->loadIdentity();
   gl->lookAt(0,10,15, 0,0,0,  0,1,0);
   gl->translate(-4,-4,0);
   gl->rotate(ang, 0, 1, 1);
   gl->scale(3,3,3);
   shader1->setMatrix(loc_u_modelview,  gl->getModelViewMatrix()  );
   shader1->setMatrix(loc_u_projection, gl->getProjectionMatrix() );
   glutSolidIcosahedron();

   //glutSolidTeapot is not an appropriate primitive to compare fixed and programmable functionalities of OpenGL.
   //It uses glScale(size) and glRotate() to position and scale the teapot on the screen. Those
   //transformations cannot be processed when using OpenGL 3.2. So I decided to use glutSolidIcosahedron
   //You can use glGet(GL_MODELVIEW_MATRIX, params), for example, to return the value of the GL_MODELVIEW_MATRIX to check
   //if the MyGL->m_modelViewMatrix has the same values.
   //glutSolidTeapot(size);






   //444444444444444444444444444444444444444444444444444444444444444444444444444
   //------------------------------------------------------------------------
   //uses shader and the OpenGL matrices to specify camera, viewing and projection
   //===================================================================
   shader2->setActive(true);
   glLoadIdentity();
   gluLookAt( 0,10,15, 0,0,0,  0,1,0);
   glTranslated(4,-4,0);
   glRotated( ang, 0, 1, 1);
   glScalef( 3,3,3);
   glutSolidIcosahedron();
   shader2->setActive(false);

   /**/

   ang+=0.01f;
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   gl->matrixMode(GL_PROJECTION);

   glLoadIdentity();
   gl->loadIdentity();

   glOrtho (-10, 10,  -10, 10,  -100.0, 100.0);
   gl->ortho(-10, 10,  -10, 10,  -100.0, 100.0);

   //gluPerspective(80,1, 1, 100);
   //gl->perspective(80,1, 1, 100);
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (400, 400);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("OpenGL 1.0 vs OpenGL 2.0 vs OpenGL 3.2");
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);

   init_gl();

   gl = new MyGL();

   shader1 = new Glsl("1.vert", "1.frag");
   shader2 = new Glsl("2.vert", "2.frag");

   loc_u_projection = shader1->getUniformLoc("projectionMatrix");
   loc_u_modelview  = shader1->getUniformLoc("modelViewMatrix");

   //unsigned int id;
   //glGenBuffers(1, &id);

   //printf("\n%d %d ",  loc_u_projection, loc_u_modelview);

   glutMainLoop();
   return 0;
}
