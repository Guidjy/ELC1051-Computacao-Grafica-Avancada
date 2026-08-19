

/**
*   Programa para ilustrar:
*   - uso de vertex e fragment shader
*   - tratamento de erros do opengl
*   - erros nos shaders

*   Autor/Adaptacao: Cesar Tadeu Pozzer
*   UFSM - 09/2008
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

#include "glsl.h"


//variaveis uniform
float u_time_1=0;  //tempo decorrido = animacao
float u_time_2=0;  //tempo decorrido = animacao
GLint loc_u_time_1;  //local da variavel uniform do arquivo tea_1.vert
GLint loc_u_time_2;  //local da variavel uniform do arquivo tea_2.vert

Glsl *shader1, *shader2;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   //GLuint i;
   //glGenBuffers(1, &i);

   glShadeModel (GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   glRotated(30, 1, 1, 0);
   u_time_1+=0.2;
   u_time_2+=0.1;

   //seta a variavel time para todo o teapot (uniform = pouca alteracao).
   shader2->setActive(false);
   shader1->setActive(true);
   glUniform1f(loc_u_time_1, u_time_1/10);
   glutSolidTeapot(4);

   int v[1];
   glGetIntegerv(GL_CURRENT_PROGRAM, v);
   //printf("\rPrograma ativo: %d", v[0]);

   shader1->setActive(false);
   shader2->setActive(true);
   glUniform1f(loc_u_time_2, u_time_2/10);
   glTranslated(4,6,0);
   glutSolidTeapot(3);

   shader1->setActive(false);
   shader2->setActive(false);
   glTranslated(-12,0,0);
   glutSolidTeapot(3);


   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho (-10, 10,  -10,10,  -10.0, 10.0);  //projecao ortografica

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

   init_gl();

   shader1 = new Glsl("glsl_3_Vertex\\src\\tea_1.vert", "glsl_3_Vertex\\src\\tea_1.frag");
   shader2 = new Glsl("glsl_3_Vertex\\src\\tea_2.vert", "glsl_3_Vertex\\src\\tea_2.frag");
   loc_u_time_1 = shader1->getUniformLoc("time");
   loc_u_time_2 = shader2->getUniformLoc("time");

   printf("%d %d", loc_u_time_1, loc_u_time_2);

   glutMainLoop();
   return 0;
}
