//*********************************************************************
//
// Programa para ilustrar uso de:
//             - modelos MD2
//             - textura
//             - BMP
//             - iluminacao
// Autor: Cesar Tadeu Pozzer
// pozzer@inf.ufsm.br
// 2007/2
//
//*********************************************************************

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "Bmp.h"
#include "Actor.h"
#include "md2.h"

#define SCREEN_X 500
#define SCREEN_Y 500

#define MODELO  1
#define NORMAIS 2

int  polygonMode = 1;
int action = MODELO;
float ang = 0;

Actor *actor = NULL;

GLfloat mat_specular[] = { 1, 1, 1, 1 };
GLfloat mat_shininess[] = { 10 };

GLfloat light_0_position[] = { 1, 7, 1, 1 };
GLfloat light_0_difuse[]   = { 0.5, 0.5, 0.50 };
GLfloat light_0_specular[] = { 0, 0, 1 };
GLfloat light_0_ambient[]  = { 0.2, 0.2, 0.2 };

// table de 162 vecteurs normaux
static vec3_t   vertexNormals[ NUMVERTEXNORMALS ] = {
   #include   "anorms.h"
};


void initGL()
{
   glClearColor(0.5, 0.5, 0.5, 1);

   glShadeModel(GL_SMOOTH);
   glDepthFunc(GL_LESS);

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glDisable(GL_TEXTURE);

   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_0_difuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
   glLightfv(GL_LIGHT0, GL_AMBIENT,  light_0_ambient);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

//faz a geracao da cena. Esta funcao e' chamada apenas no inicio da execucao.
void init()
{
   actor   = new Actor();
   actor   = new Actor();

   actor->loadModel("gl_5_md2viewer/data/", "caveira.md2", "caveira.bmp");
   actor->loadWeapon("gl_5_md2viewer/data/", "caveiraWeapon.md2", "caveiraWeapon.bmp");

   //actor->loadModel("gl_5_md2viewer/data/", "goblin.md2", "goblin2.bmp");
   //actor->loadWeapon("gl_5_md2viewer/data/", "goblinWeapon.md2", "goblinWeapon.bmp");

   //actor->loadModel("./data/", "ogros.md2", "ogros.bmp");
   //actor->loadWeapon("./data/", "ogrosWeapon.md2", "ogrosWeapon.bmp");

   //actor->loadModel("./data/", "mask.md2", "mask.bmp");
   //actor->loadWeapon("./data/", "maskWeapon.md2", "maskWeapon.bmp");

   //actor->loadModel("./data/", "yohko.md2", "yohko.bmp");
   //actor->loadWeapon("./data/", "yohkoWeapon.md2", "yohkoWeapon.bmp");

   //actor->loadModel("./data/", "spiff.md2", "spiff.bmp");
   //actor->loadWeapon("./data/", "spiffWeapon.md2", "spiffWeapon.bmp");

   //actor->loadModel("./data/", "pknight.md2", "spiff.bmp");
   //actor->loadWeapon("./data/", "pknightWeapon.md2", "spiffWeapon.bmp");

   //actor->loadModel("./data/", "phantom.md2", "phantom2.bmp");
   //actor->loadWeapon("./data/", "phantomWeapon.md2", "phantomWeapon.bmp");

   //actor->loadModel("./data/", "orc.md2", "orc.bmp");
   //actor->loadWeapon("./data/", "orcWeapon.md2", "orcWeapon.bmp");

   //actor->loadModel("./data/", "solo.md2", "solo.bmp");
   //actor->loadWeapon("./data/", "soloWeapon.md2", "soloWeapon.bmp");

   //actor->loadModel("./data/", "marine.md2", "marine.bmp");
   //actor->loadWeapon("./data/", "marineWeapon.md2", "marineWeapon.bmp");

   //actor->loadModel("./data/", "robo.md2", "robo.bmp");
   //actor->loadWeapon("./data/", "roboWeapon.md2", "roboWeapon.bmp");

   //actor->loadModel("./data/", "dragon.md2", "dragon.bmp");

   //actor->loadModel("./data/", "rhino.md2", "rhino.bmp");

   actor->scale(1.2);
}

//desenha os vetores normais definidos no arquivo anorms.h.
void renderNormais()
{
	glScalef(40,40,40);
	glRotatef(ang+=0.1, 1, 0, 1);
	glBegin(GL_LINES);
	for(int i=0; i<NUMVERTEXNORMALS; i++)
	{
	   glVertex3f(0,0,0);
	   glVertex3fv( vertexNormals[i] );
	}
   glEnd();

}


////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
   float abertura = 85.0;
   float znear  = 1;
   float zfar   = 20000;
   float aspect = 1;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(abertura, aspect, znear, zfar);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity( );

   gluLookAt(0,0, 50, 0,0,0, 0,1,0);
   glRotated(ang+=0.1, 0, 1, 0);


   //seleciona o que renderizar
	if( action == MODELO )
	    actor->render();
	else if ( action == NORMAIS )
      renderNormais();

	glutSwapBuffers();
}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   printf("%c", key);
	key = tolower(key);
   switch(key)
   {
      case 27:
         exit(0);
      break;

      case 'w': //wireframe
         if(polygonMode==1)
         {
            polygonMode=0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         }
         else
         {
            polygonMode=1;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
         break;

		case 'm': //modelo
         action = MODELO;
			break;

		case 'n': //normais
         action = NORMAIS;
			break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutInitWindowPosition (450, 10);
   glutCreateWindow ("MD2 Viewer. Use M-modelo, N-normais");

   initGL();
   init();

   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutKeyboardFunc(keyboard);

   glutMainLoop();
   return 0;
}
