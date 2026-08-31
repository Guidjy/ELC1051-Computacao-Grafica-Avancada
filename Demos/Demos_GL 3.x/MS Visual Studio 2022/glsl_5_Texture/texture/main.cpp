/**
*   Programa para ilustrar:
*   - Acesso a textura em GLSL
*   - comando glActiveTexture()
*
*
*   Autor/Adaptacao: Cesar Tadeu Pozzer
*   UFSM - 09/2007
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "Bmp.h"
#include "glsl.h"

#define RECT_SIZE 10

const double PI = 3.14159265358979323846;

//variaveis uniform
GLint loc_u_texture_0;  //local da variavel texture do arquivo tex.frag
GLint loc_u_texture_1;  //local da variavel texture do arquivo tex.frag
GLint loc_u_bright;     //local da variavel bright do arquivo tex.frag

float brilho = 0;
float mouseX = 0.0f;
float mouseY = 0.0f;
int windowHeight = 600;
int pressedKey;

float rotationAngle = 0.0;

Bmp *img1, *img2;
unsigned char *data1, *data2;
GLuint tex1, tex2;

float ang = 0;

Glsl *shader1;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glShadeModel (GL_SMOOTH);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   img1 = new Bmp("resource\\lena.bmp");
   img1->convertBGRtoRGB();
   data1 = img1->getImage();
   if( data1 == NULL )
   {
       printf("erro");
	   exit(0);
   }
   img2 = new Bmp("resource\\textura.bmp");
   img2->convertBGRtoRGB();
   data2 = img2->getImage();
   if( data2 == NULL )
   {
       printf("erro");
	   exit(0);
   }
   //glEnable(GL_TEXTURE);
   //glEnable(GL_TEXTURE_2D);

   glGenTextures( 1, &tex1 );
   glBindTexture( GL_TEXTURE_2D, tex1 );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1->getWidth(),img1->getHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, data1);

   glGenTextures( 1, &tex2 );
   glBindTexture( GL_TEXTURE_2D, tex2 );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img2->getWidth(),img2->getHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, data2);


   //glActiveTexture selects which texture unit subsequent texture state calls will
   //affect.  The number of texture units an implementation supports is
   //implementation dependent, but must be at least 2.
   glActiveTexture(GL_TEXTURE0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture( GL_TEXTURE_2D, tex1 );

   glActiveTexture(GL_TEXTURE1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture( GL_TEXTURE_2D, tex2 );


}

void display(void)
{
   Sleep(5);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   //muda a intensidade da cor de cada pixel por uma senoide no intervalo [0.2, 4.2]
   float valor = (sin(brilho+=0.01)+1.1)*2.0;
   int texture = (valor>0.5)? 0:1;

   glUniform1f(loc_u_bright, valor);
   glUniform1i(loc_u_texture_0, 0);
   glUniform1i(loc_u_texture_1, 1);

   // pega localização em memória da uniforme "mouse" no GPU
   GLint mouseLocation = shader1->getUniformLoc("mouse");
   // manda a posição do mouse no CPU para o GPU
   if (mouseLocation != -1)
   {
       glUniform2f(mouseLocation, mouseX, mouseY);
   }

   GLint pressedKeyLocation = shader1->getUniformLoc("pressedKey");
   if (pressedKeyLocation != -1)
   {
       glUniform1i(pressedKeyLocation, pressedKey);
   }

   GLint rotationOffsetLocation = shader1->getUniformLoc("rotationAngle");
   if (rotationOffsetLocation != -1)
   {
       glUniform1f(rotationOffsetLocation, rotationAngle);
   }

   rotationAngle += 0.01;
   if (rotationAngle >= PI * 2) rotationAngle = 0.0;

   glNormal3f(0, 1, 0);
   glBegin(GL_QUADS);

      glTexCoord2f(0, 0);
      glVertex3f(-2, -2,  -10);

      glTexCoord2f(1, 0);
	  glVertex3f(2, -2,   -10);

      glTexCoord2f(1, 1);
      glVertex3f(2, 2,  -10);

      glTexCoord2f(0, 1);
      glVertex3f(-2, 2, -10);
   glEnd();

   glutSwapBuffers();
  // printf("x");
}

void reshape (int w, int h)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, 1, 1, 100);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouseMotion(int x, int y)
{
    mouseX = (float)x;
    // inverte eixo Y: GLUT seta (0, 0) no top-left, mas o OpenGL seta (0, 0) no bottom-left
    mouseY = (float)(windowHeight - y);
}

void keyboard(unsigned char key, int x, int y)
{
    pressedKey = (int)key;
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
   glutPassiveMotionFunc(mouseMotion);
   glutMotionFunc(mouseMotion);
   glutKeyboardFunc(keyboard);

   //neste caso, o glsl deve ser inicializado antes do gl para poder chamar o
   //comando  glActiveTexture();
   shader1 = new Glsl("tex.vert", "tex.frag");
   shader1->setActive(true);
   init_gl();

   loc_u_texture_0 = shader1->getUniformLoc("texture_0");
   loc_u_texture_1 = shader1->getUniformLoc("texture_1");
   loc_u_bright    = shader1->getUniformLoc("brilho");

   //printf(" IDs: %d %d ", loc_u_texture, loc_u_bright);

   glutMainLoop();
   return 0;
}
