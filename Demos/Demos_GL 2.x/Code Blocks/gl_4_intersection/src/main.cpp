/**
*   Programa para ilustrar:
*   - calculo de intersecao entre linhas
*   - calculo de intersecao entre linha e Circulo

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 1/11/2007
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "Vector.h"
#include "Line.h"

Vector p0(200,200), d0, p1(200,90), d1;

Line l1(p0, d0);
Line l2(p1, d1);
Circle c1(p0, 100);


float ang1 = 0;
float ang2 = 2;

void init()
{
   glClearColor(1,1,1,1);
   glShadeModel(GL_SMOOTH);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0, 500, 0, 500);

   glMatrixMode(GL_MODELVIEW);
}

void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT );
   glColor3f(1, 0, 0);

   float x = cos(ang1)*200;
   float y = sin(ang1)*200;

   ang1+=0.0123;
   d0.x = x;
   d0.y = y;

   x = cos(ang2+1.5)*300;
   y = sin(ang2+1.5)*300;
   ang2+=0.02;
   d1.x = x;
   d1.y = y;

   l1.setDir(d0);
   l2.setDir(d1);
   float s, t;
   bool b;

   l1.render();
   l2.render();
   c1.render();

   b = l1.intersects(l2, s, t);
   l1.renderIntersection(s, b);

   b = l1.intersects(c1, s, t);
   l1.renderIntersection(s, b);
   l1.renderIntersection(t, b);

   b = l2.intersects(c1, s, t);
   l2.renderIntersection(s, b);
   l2.renderIntersection(t, b);

   glutSwapBuffers();
}

int main (int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (250, 100);
	glutCreateWindow ("Teste de Intersecao entre linhas e Circulos" );
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}
