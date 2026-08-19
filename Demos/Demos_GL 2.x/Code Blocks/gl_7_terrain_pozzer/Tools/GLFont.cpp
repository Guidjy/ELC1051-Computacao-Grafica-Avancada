// GLFont.cpp: implementation of the GLFont class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include "GLFont.h"
//#include <gl/glu.h>
#include "./gl/glut.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLFont::GLFont()
{

}

GLFont::~GLFont()
{

}


void GLFont::startText()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, 800.0f, 0, 600.0f);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -580.0f, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void GLFont::endText()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void GLFont::print(float x, float y, char *string)
{

	char *c;
	int x1=x;
	for (c=string; *c != '\0'; c++) {
		glRasterPos2f(x1,y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x1 = x1 + glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,*c) + 10;
	}
}

void GLFont::printShadow(float x, float y, char *string)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	print(x+1, y+1, string);
	glColor3f(0.0f, 0.0f, 0.0f);
	print(x, y, string);
}
