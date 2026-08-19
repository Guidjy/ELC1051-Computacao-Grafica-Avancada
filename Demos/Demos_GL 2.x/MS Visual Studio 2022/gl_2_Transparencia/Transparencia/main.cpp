/**
*   Programa para ilustrar:
*   - Uso da textura com transparencia (canal Alpha)
*   - Formato TGA com transparencia
*   - Formato BMP com adicao de transparencia
*   - Mescla de cores e textura
*
*   Autor: Cesar Tadeu Pozzer
*   UFSM - 28/08/2012
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#include "Bmp.h"
#include "tga.h"

#define SCREEN_X 600
#define SCREEN_Y 600


Bmp *img1, *img2;
GLuint textureID[3];

void buildTextures();
bool LoadTexture(const char *TexName, GLuint TexHandle);

float g_z = 0; //para uso de trnasparencia, os objetos devem ser desenhados de traz para frente - algoritmo do pintor.
float ang = 0;


////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
    g_z = 0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0, SCREEN_X, 0, SCREEN_Y, -100,100);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity( );

   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glAlphaFunc(GL_GEQUAL, 0.0f);
   glEnable(GL_ALPHA_TEST);


    //desenha o background
    //==========================================================
    glBindTexture( GL_TEXTURE_2D, textureID[0] );
    glColor3f(1,1,1); //deve ser branco para fazer blending com a textura sem mudar a cor.

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, g_z);

        glTexCoord2f(1, 0);
        glVertex3f(SCREEN_X, 0, g_z);

        glTexCoord2f(1, 1);
        glVertex3f(SCREEN_X, SCREEN_Y, g_z);

        glTexCoord2f(0, 1);
        glVertex3f(0, SCREEN_Y, g_z);
    glEnd();
    g_z++; //incrementa o z para desenho do proximo objeto

    //desenha uma textura transparente sobre a base
    //==========================================================
    glBindTexture( GL_TEXTURE_2D, textureID[1] );
    glColor3f(1,1,1); //deve ser branco para fazer blending com a textura sem mudar a cor.

    glPushMatrix();
    glTranslated(300,350,0);
    glRotated(-ang++, 0,0,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-300, 300, g_z);

        glTexCoord2f(1, 0);
        glVertex3f(300, 300, g_z);

        glTexCoord2f(1, 1);
        glVertex3f(300, -300, g_z);

        glTexCoord2f(0, 1);
        glVertex3f(-300, -300, g_z);
    glEnd();
    glPopMatrix();
    g_z++;

    //desenha uma textura transparente sobre a ultima textura
    //==========================================================
    glBindTexture( GL_TEXTURE_2D, textureID[2] );
    glColor3f(1,1,1); //deve ser branco para fazer blending com a textura sem mudar a cor.

    glPushMatrix();
    glTranslated(300,200,0);
    glRotated(ang++, 0,0,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-200, 200, g_z);

        glTexCoord2f(1, 0);
        glVertex3f(200, 200, g_z);

        glTexCoord2f(1, 1);
        glVertex3f(200, -200, g_z);

        glTexCoord2f(0, 1);
        glVertex3f(-200, -200, g_z);
    glEnd();
    glPopMatrix();
    g_z++;

    //desenha linha sem textura
    //==========================================================
    glDisable(GL_TEXTURE_2D); //deve-se desabilitar a textura_2D para nao misturar com a cor da linha
    glColor3f(1,0,1);
    glLineWidth(12);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0, g_z);
        glVertex3f(500.0, 300.0, g_z);
    glEnd();
    //deve-se restaurar a cor branca (default) para o blending com a textura nao mudar a cor dela
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);

    Sleep(10);
    glutSwapBuffers();
}



void init()
{
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);

    buildTextures();
}


void buildTextures()
{
   //cria varios ids de textura e associa com cada imagem carregada do arquivo
   glGenTextures( 4, textureID );

   img1 = new Bmp("resources\\background.bmp");
   img1->convertBGRtoRGB();
   img1->createTexture(textureID[0]);

   img2 = new Bmp("resources\\tank.bmp");
   img2->convertBGRtoRGB();
   img2->addAlpha();
   img2->createTexture(textureID[1]);

   LoadTexture("resources\\ColWheel.tga", textureID[2]);
}

// Load a TGA texture
//http://content.gpwiki.org/index.php/OpenGL:Tutorials:Tutorial_Framework:Ortho_and_Alpha
bool LoadTexture(const char *TexName, GLuint TexHandle)
{
  TGAImg Img;        // Image loader

  // Load our Texture
   if(Img.Load(TexName)!=IMG_OK)
    return false;

  glBindTexture(GL_TEXTURE_2D,TexHandle); // Set our Tex handle as current

  // Create the texture
   if(Img.GetBPP()==24)
    glTexImage2D(GL_TEXTURE_2D,0,3,Img.GetWidth(),Img.GetHeight(),0,GL_RGB,GL_UNSIGNED_BYTE,Img.GetImg());
   else if(Img.GetBPP()==32)
    glTexImage2D(GL_TEXTURE_2D,0,4,Img.GetWidth(),Img.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,Img.GetImg());
   else
    return false;

   unsigned char *p = Img.GetImg();
   printf("\n%s:  %d %d %d %d", TexName, Img.GetBPP(), *p, *(p+1), *(p+1));

  // Specify filtering and edge actions
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  return true;
 }

////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Transparent Texture: BMP and TGA");
   init();

   glutDisplayFunc(display);
   glutIdleFunc(display);

   glutMainLoop();
   return 0;
}
