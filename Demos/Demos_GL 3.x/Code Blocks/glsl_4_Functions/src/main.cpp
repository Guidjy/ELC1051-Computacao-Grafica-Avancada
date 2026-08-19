/**
*   Programa para ilustrar:
*   - Declaracao e inicializacao de variaveis em shaders
*   - Uso de built-in functions da biblioteca GLSL
*   - Depuracao de codigo de shader

*   Autor/Adaptacao: Cesar Tadeu Pozzer
*   UFSM - 02/2008
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "glsl.h"


Glsl *shader1;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glShadeModel (GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   shader1->setActive(true);

   glLoadIdentity();

   glBegin(GL_QUADS);
      glVertex3f(-2, -2,  -10);
	  glVertex3f(2, -2,   -10);
      glVertex3f(2, 2,  -10);
      glVertex3f(-2, 2, -10);
   glEnd();

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, 1, 1, 100);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("Shader 1");
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);

   shader1 = new Glsl("glsl_4_Functions\\src\\func.vert", "glsl_4_Functions\\src\\func.frag");
   init_gl();


   glutMainLoop();
   return 0;
}
