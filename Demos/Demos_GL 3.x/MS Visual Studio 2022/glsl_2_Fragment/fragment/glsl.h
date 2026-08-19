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
   Glsl(const char *vert, const char *frag);
   void setActive(bool);
   GLint getUniformLoc(const char *);
   GLint getAttribLoc(const char *);

private:
   void setShaders(const char *vert, const char *frag);
   char *textFileRead(const char *fn);
   int  printOglError(const char *file, int line);
   void printProgramInfoLog(GLuint obj);
   void printShaderInfoLog(GLuint obj);
};