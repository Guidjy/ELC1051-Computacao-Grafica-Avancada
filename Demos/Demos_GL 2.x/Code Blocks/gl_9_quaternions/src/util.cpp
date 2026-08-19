
#include <GL/glut.h>
#include "global.h"


void eixos()
{
	glColor3f(0.5, 0.5, 0.5);		// origin
	glutWireCube(1.0);

	glPushMatrix();			// x axis
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(5, 0, 0);
	glEnd();
	glTranslatef(5,0,0);	
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();			// y axis
	glColor3f(0, 0.5, 0.5);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 5, 0);
	glEnd();
	glTranslatef(0,5,0);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();			// z axis
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 5);
	glEnd();
	glTranslatef(0,0,5);
	glutWireCube(1);
	glPopMatrix();
}

void eixos_2()
{
	glColor3f(0.5, 0.5, 0.5);		// origin

	glPushMatrix();			// x axis
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(5, 0, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();			// y axis
	glColor3f(0, 0.5, 0.5);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 5, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();			// z axis
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 5);
	glEnd();
	glPopMatrix();
}

void eixos_3(Vector3 eixo)
{
   glColor3f(1, 1, 0);
   glBegin(GL_LINES);
      glVertex3f(0, 0, 0);
      glVertex3f(eixo.x*5, eixo.y*5, eixo.z*5);
   glEnd();
 //axis     
} 


void text(int x, int y, char *t)
{
   int tam = strlen(t);

   for(int c=0; c<tam; c++)
   {
      glRasterPos2i(x + c*10, y);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, t[c]);
   }
}


