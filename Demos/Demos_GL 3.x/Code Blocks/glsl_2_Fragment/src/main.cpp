/**
*   Programa para ilustrar:
*   - uso de vertex e fragment shader
*   - tratamento de erros do opengl
*   - uso de variaveis uniform e attribute
*   - processamento de cor no fragment shader

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 12/2007
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


float u_time=0;  //tempo decorrido = animacao
int   u_mode=3;  //modo de exibicao

GLint loc_a_cor;   //local da variavel attribute do arquivo hello.vert
GLint loc_a_scalar;  //local da variavel attribute do arquivo hello.vert
GLint loc_u_time;  //local da variavel uniform do arquivo hello.frag
GLint loc_u_mode;  //local da variavel uniform do arquivo hello.frag

float randomico = 0;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   //glShadeModel (GL_SMOOTH);
   glShadeModel (GL_FLAT);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   shader1->setActive(true);

   Sleep(10);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0, 200, 0, 200);
   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();

   u_time+=0.1;
   glUniform1f(loc_u_time, u_time);
   glUniform1i(loc_u_mode, u_mode);

   //seta a variavel time para todo o teapot (uniform = pouca alteracao).
   glBegin(GL_QUADS);
     glVertexAttrib3f(loc_a_cor, 1.0*((sin(randomico+=0.01)+1.2)/2.0), 0, 0);
     glVertexAttrib1f(loc_a_scalar, 0);
     glVertex2f(0, 0);

     glVertexAttrib3f(loc_a_cor, 0, 1*((cos(1+randomico)+1.1)/2.0), 0);
     glVertexAttrib1f(loc_a_scalar, 0.3);
     glVertex2f(200, 0);

     glVertexAttrib3f(loc_a_cor, 0, 0, 1);
     glVertexAttrib1f(loc_a_scalar, 0.8);
     glVertex2f(200, 200);

	 glVertexAttrib3f(loc_a_cor, 0, 1, 0);
     glVertexAttrib1f(loc_a_scalar, 0.2);
     glVertex2f(0, 200);
   glEnd();

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, w, 0, h);  //projecao ortografica

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char c, int x, int y)
{
   if ( c == '1' )
      u_mode = 1;
   else if (c == '2' )
	  u_mode = 2;
   else if (c == '3' )
	  u_mode = 3;
   else
      u_mode = 4;
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("Fragment Shader");
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   init_gl();

   shader1 = new Glsl("glsl_2_Fragment\\src\\hello.vert", "glsl_2_Fragment\\src\\hello.frag");

   loc_a_cor    = shader1->getAttribLoc("cor");
   loc_a_scalar = shader1->getAttribLoc("scalar");
   loc_u_time   = shader1->getUniformLoc("time");
   loc_u_mode   = shader1->getUniformLoc("mode");

   printf("%d %d %d %d ", loc_a_cor, loc_a_scalar, loc_u_time, loc_u_mode);

   glutMainLoop();
   return 0;
}
