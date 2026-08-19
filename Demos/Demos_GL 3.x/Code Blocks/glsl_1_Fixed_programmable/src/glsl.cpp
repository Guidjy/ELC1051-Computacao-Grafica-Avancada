
//Author: http://www.lighthouse3d.com/opengl/glsl/index.php
//
//---------------------------------------------
// Adaptation: Cesar Tadeu Pozzer
// UFSM - 06/2010
//---------------------------------------------
//
// Version 1.1
//

#include "glsl.h"


//Only one shading program can be active at a given time
//Pozzer
//----------------------------------------------
void Glsl::setActive(bool active)
{
   if(active == true)
   {
	  glUseProgram(p);
	  isActive = true;
   }
   else
   {
	  glUseProgram(0); //disable the current shading program
	  isActive = false;
   }
}

//Pozzer
//----------------------------------------------
GLint Glsl::getUniformLoc(char *nome)
{
   GLint id = glGetUniformLocation(p, nome);
   if( id == -1 )
	  printf("\nError: Variable %s not defined or unused", nome);
   return id;
}

//Pozzer
//----------------------------------------------
GLint Glsl::getAttribLoc(char *nome)
{
   GLint id = glGetAttribLocation(p, nome); 
   if( id == -1 )
	  printf("\nError: Variable %s not defined or unused", nome);
   return id;
}

//Pozzer
//----------------------------------------------
void Glsl::setMatrix(GLint loc, GLfloat *ptr)
{
   if(isActive)
   {
      glUniformMatrix4fv(loc, 1, GL_FALSE, ptr);
   }
   else
	  printf("Error: Shader not active");
}
  

void Glsl::setShaders(char *vert, char *frag) 
{
   char *vs,*fs;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);   

   //printf("%d %d ", v, f);

   vs = textFileRead(vert);
   fs = textFileRead(frag);

   const char * vv = vs;
   const char * ff = fs;
 
   glShaderSource(v, 1, &vv, NULL);
   glShaderSource(f, 1, &ff, NULL);

   free(vs);
   free(fs);

   glCompileShader(v);
   glCompileShader(f);

   p = glCreateProgram();
   
   glAttachShader(p,v); //A program can have 0 or more shades. If it has just the vertex shader, for example, fixed functionality is used for the fragment shader. 
   glAttachShader(p,f); 

   glLinkProgram(p);
   //glUseProgram(p); //user must activate the desired shader
   printf("\nProgram with id %d Ok\n", p);
}

//Pozzer
//----------------------------------------------
Glsl::Glsl(char *vert, char *frag)
{
   isActive  = false;

   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      /* Problem: glewInit failed, something is seriously wrong. */  
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }
   if (showInfo == false)
   {
	  showInfo = true;
	  printf("\nYou are using OpenGL %s\n\n", glGetString(GL_VERSION));
  
	  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
   
      if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
         printf("Ready for GLSL\n");
      else {
         printf("Not totally ready :( \n");
         exit(1);
      }
      if (glewIsSupported("GL_VERSION_2_0"))
         printf("Ready for OpenGL 2.0\n");
      else {
         printf("OpenGL 2.0 not supported\n");
         exit(1);
      }
   }

        
   setShaders(vert, frag);

   printShaderInfoLog(v);
   printShaderInfoLog(f);
   printProgramInfoLog(p);

   //http://www.delorie.com/gnu/docs/gcc/cpp_21.html
   //http://www.thescripts.com/forum/thread212429.html
   //http://www.codeguru.com/forum/showthread.php?t=231043
   //printf("\n%s  %d  %s  %s \n", __FILE__, __LINE__, __FUNCTION__, __TIMESTAMP__);

   printOglError(__FILE__, __LINE__); 
}

char *Glsl::textFileRead(char *fn) 
{
   FILE *fp;
   char *content = NULL;

   int count=0;

   if (fn != NULL) 
   {
      fp = fopen(fn,"rt");

      if (fp != NULL) 
      {
         fseek(fp, 0, SEEK_END);
         count = ftell(fp);
         rewind(fp);

         if (count > 0) {
            content = (char *)malloc(sizeof(char) * (count+1));
            count = (int)fread(content,sizeof(char),count,fp);
            content[count] = '\0';
         }
         fclose(fp);
      }
      else
      {
         printf("\nFile %s not found", fn);
         getchar();
         exit(1);
      } 
   }
   //printf("File %s %d = \n%s\n\n", fn, strlen(content), content);
   return content;
}

int Glsl::printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void Glsl::printShaderInfoLog(GLuint obj)
{
   int infologLength = 0;
   int charsWritten  = 0;
   char *infoLog;

   glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

   if (infologLength > 0)
   {
      infoLog = (char *)malloc(infologLength);
      glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
   }
}

void Glsl::printProgramInfoLog(GLuint obj)
{
   int infologLength = 0;
   int charsWritten  = 0;
   char *infoLog;

   glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

   if (infologLength > 0)
   {
      infoLog = (char *)malloc(infologLength);
      glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
   }
}