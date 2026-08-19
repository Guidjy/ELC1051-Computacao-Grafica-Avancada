/**
*   Programa para ilustrar:
*   - uso de vertex e fragment shader
*   - tratamento de erros do opengl
*   - uso de variaveis uniform e attribute
*   - processamento de cor no fragment shader

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 12/2007
*
*   http://www.lighthouse3d.com/opengl/glsl/
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "glsl.h"

#define SCREEN_X 500
#define SCREEN_Y 500

Glsl *shader1, *shader2;

GLint loc_u_light;  //local da variavel uniform do arquivo toon.vert

float u_lightPos[3] = {0,0,3};
float ang = 0;

float rx = 0, rz = 0;

void init_gl()
{
   float abertura = 45.0;
   float znear  = 1;
   float zfar   = 20;
   float aspect = 1;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glEnable(GL_DEPTH_TEST);

}

////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity( );


   //move a posicao da luz
   float x, z;
   x = cos(ang);
   z = sin(ang);
   u_lightPos[0] = x;
   u_lightPos[2] = z;
   ang+=0.001;

   glUniform3f(loc_u_light, u_lightPos[0], u_lightPos[1], u_lightPos[2]);

   glTranslated(0, 0, -5);
   glRotatef ((GLfloat) rx, 0.0f, 1.0f, 0.0f);
   glRotatef ((GLfloat) rz, 1.0f, 0.0f, 0.0f);
   glColor3f(0, 0, 1);
   glutSolidTeapot(1.2);

   glutSwapBuffers();
}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   key = tolower(key);
   switch(key)
   {
      case 27:
	     exit(0);
	  break;
      case '1':
        shader2->setActive(false);
        shader1->setActive(true);
	  break;
      case '2':
        shader1->setActive(false);
        shader2->setActive(true);
	  break;
   }
}

void MotionFunc(int x, int y)
{
   rx = x;
   rz = y;
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("Toon Shader: Digite 1 ou 2 para trocar o shader");
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(MotionFunc);

   init_gl();
   shader1 = new Glsl("glsl_7_Toon\\src\\toon.vert", "glsl_7_Toon\\src\\toon.frag");
   shader2 = new Glsl("glsl_7_Toon\\src\\toon2.vert", "glsl_7_Toon\\src\\toon2.frag");

   loc_u_light = shader1->getUniformLoc("lightSourcePos");
   loc_u_light = shader2->getUniformLoc("lightSourcePos");
   printf("%d ", loc_u_light);

   shader1->setActive(true);


   glutMainLoop();
   return 0;
}
