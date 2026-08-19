/**
*   Programa para ilustrar:
*   - Acesso a textura em GLSL
*   - Bump mapping aplicada em planos
*   - calculo de angulo entre vetores
*   - carregamento de imagens BMP

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 09/2008
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
#define TEXTURE_REPET 2


//variaveis uniform
GLint loc_u_light;   //local da variavel bright do arquivo tex.frag

float brilho = 0;

Bmp *img1;
unsigned char *data;
GLuint textureID;

float ang = 0;
float u_lightPos[3] = {0.0, 0.0, -9.0};

Glsl *shader1;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glShadeModel (GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   //Altere o arquivo de textura aqui.
   img1 = new Bmp("glsl_8_Bump\\resource\\normal_1.bmp");
   img1->convertBGRtoRGB();
   data = img1->getImage();
   if( data == NULL )
   {
       printf("erro");
	   exit(0);
   }

   glGenTextures( 1, &textureID );
   glBindTexture( GL_TEXTURE_2D, textureID );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGB,
                img1->getWidth(),
                img1->getHeight(),
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture( GL_TEXTURE_2D, textureID );

}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   Sleep(1);

   glLoadIdentity();

   //move a posicao da luz no plano xy
   u_lightPos[0] = cos(ang)*2;
   u_lightPos[1] = sin(ang)*2;
   ang+=0.01;


   //muda a intensidade da cor de cada pixel por uma senoide no intervalo [0.2, 4.2]
   glUniform3f(loc_u_light, u_lightPos[0], u_lightPos[1], u_lightPos[2]);

  /* glPushMatrix();
      glColor3f(1, 0, 0);

      glTranslated(u_lightPos[0],u_lightPos[1],u_lightPos[2]);
      glutSolidSphere(1, 10, 10);
   glPopMatrix();*/

   //a normal nao interessa pois considera-se pois considera-se que o objeto esta sempre no plano xy
   //glNormal3f(0, 1, 0);
   glBegin(GL_QUADS);

      glTexCoord2f(0, 0);
      glVertex3f(-2, -2,  -10);

      glTexCoord2f(TEXTURE_REPET, 0);
	  glVertex3f(2, -2,   -10);

      glTexCoord2f(TEXTURE_REPET, TEXTURE_REPET);
      glVertex3f(2, 2,  -10);

      glTexCoord2f(0, TEXTURE_REPET);
      glVertex3f(-2, 2, -10);
   glEnd();

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, 1, 1, 100);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (300, 100);
   glutCreateWindow("Shader 1");
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);

   //neste caso, o glsl deve ser inicializado antes do gl para poder chamar o
   //comando  glActiveTexture();
   shader1 = new Glsl("glsl_8_Bump\\src\\tex.vert", "glsl_8_Bump\\src\\tex.frag");
   shader1->setActive(true);

   init_gl();

   loc_u_light  = shader1->getUniformLoc("lightSourcePos");

   glutMainLoop();
   return 0;
}
