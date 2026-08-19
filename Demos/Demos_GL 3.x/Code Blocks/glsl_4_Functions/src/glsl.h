//---------------------------------------------
// Autor: Cesar Tadeu Pozzer
// UFSM - 2009
//---------------------------------------------

#include <GL/glew.h>

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

class Glsl
{
   GLuint p, f, v;
public: 
   Glsl(char *vert, char *frag);
   void setActive(bool);
   GLint getUniformLoc(char *);
   GLint getAttribLoc(char *);

private:
   void setShaders(char *vert, char *frag);
   char *textFileRead(char *fn);
   int  printOglError(char *file, int line);
   void printProgramInfoLog(GLuint obj);
   void printShaderInfoLog(GLuint obj);
};