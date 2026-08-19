//---------------------------------------------
// Author: Cesar Tadeu Pozzer
// UFSM - 06/2010
// http://www-usr.inf.ufsm.br/~pozzer/
//
// Version 1.1
//---------------------------------------------

#include <GL/glew.h>

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>


static bool showInfo = false;

class Glsl
{
   GLuint p, f, v;
   bool isActive;
public: 
   Glsl(const char *vert, const char *frag);
   void setActive(bool b);
   GLint getUniformLoc(const char *);
   GLint getAttribLoc(const char *);

   void setMatrix(GLint loc, GLfloat *p);  //sets a given matrix to the shader program
 
private:
   void setShaders(const char *vert, const char *frag);
   char *textFileRead(const char *fn);
   int  printOglError(const char *file, int line);
   void printProgramInfoLog(GLuint obj);
   void printShaderInfoLog(GLuint obj);
};