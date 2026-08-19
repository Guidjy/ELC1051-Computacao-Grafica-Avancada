#ifdef _WIN32
	#include <windows.h>
#endif

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "./tools/gl/glut.h" //gl utility toolkit
#include "./tools/Frames.h"
#include "./tools/Timer.h"
#include "./tools/GLFont.h"
#include "./tools/textura.h"
#include "defs.h"
#include "Camera.h"
#include "VLTerrain.h"

//variaveis globais
Timer       *timer;
GLFont      *font;
Frames      *frames;
int         polygonMode=1;
float       mousex, mousey;
Camera      *camera;
VLTerrain   *terrain;


float rotacao   = 0.0;
bool  arrayList = true;
bool  debug = false;
bool  texture = false;
bool  cullingB = false;
int   cont_erros=0;

int oldX = 0, oldY = 0;
int max_render   = 0; //usado pelo mouse

int TriangleCounter;

//faz a geracao da cena. Esta funcao e' chamada apenas no inicio da execucao.
void init()
{
   frames     = new Frames();
   font       = new GLFont();
   timer      = new Timer();
   camera     = new Camera();

   terrain = new VLTerrain();
   terrain->setCamera( camera );
   
   mousex=mousey=0;

   //inicializacao do OpenGL
   glClearColor(0, 0, 0, 1);
   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //glPolygonMode(GL_FRONT, GL_LINE);


   glVertexPointer (3, GL_FLOAT, 0, terrain->coordVert);     //coord per vertex, coord type, offset, pointer 
		
   glEnableClientState(GL_VERTEX_ARRAY);

 	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   
   float escala = 1.0 / (DIM_TERRAIN*STEP); 
	float	p[4] = { escala, 0, 0, 0 };
	glTexGenfv(GL_S, GL_OBJECT_PLANE, p);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	p[0] = 0;	p[2] = escala;
	glTexGenfv(GL_T, GL_OBJECT_PLANE, p);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

  	glEnable(GL_TEXTURE_2D);
}

void teste(void)
{
    float lixo = 0;
    for(int i=0; i<5000; i++)
    {
        //lixo += sin((i)/102847.8763;
       // lixo -= tan(i+1)/147.8763;
       // lixo += cos(i+1)/147.8763;
    }
    //printf("%0.1f ", lixo);
}


////////////////////////////////////////////////////////////////////////////////////////
void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);

   camera->render( );

    //VISAO PLANAR EM XY
    //glTranslated( -DIM_TERRAIN*STEP/2, DIM_TERRAIN*STEP/2, -DIM_TERRAIN*STEP/2-4000);
    //glRotatef(90, 1.0f, 0.0f, 0.0f);

   //glRotated(rx, 1, 0, 0);
   //glRotated(rz, 0, 0, 1);
   
   //glRotatef(90, 1.0f, 0.0f, 0.0f);
   //glTranslated( -viewer[0], -viewer[1], -viewer[2] );

   glColor3f(1, 1, 1);
   terrain->renderMain();
   
   if( terrain->contErro > 0)
      printf(" %d", terrain->contErro);
   //teste();    


   static char text[50];
   float fps = frames->getFrames();

   glColor3f (1,0,0);
   font->startText();
   //glColor3f(1,1,1);
   sprintf(text, "Fps: %.2f ", fps);
   font->print(20.0f, 15.0f, text);
   sprintf(text, "TRI: %d ", terrain->triangulos);
   font->print(20.0f, 45.0f, text);
   font->endText();
   TriangleCounter += terrain->triangulos;
   glutSwapBuffers();
}



//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   //printf("%c", key);
   key = tolower(key);
   switch(key)
   {
      case 27:
         exit(0);
      break;

      case '-': 
         terrain->ThresholdDetail+=0.1;
         if( terrain->ThresholdDetail > 170 )
            terrain->ThresholdDetail = 170;
         printf("\nThresholdDetail = %.2f",    terrain->ThresholdDetail);
      break;
 
      case '+': 
         terrain->ThresholdDetail-=0.1;
         if( terrain->ThresholdDetail < 0.01 )
            terrain->ThresholdDetail = 0.01;
         printf("\nThresholdDetail = %.2f",    terrain->ThresholdDetail);
      break;

      case 'w': //wireframe
         if(polygonMode==1)
         {
            polygonMode=0;
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glPolygonMode(GL_BACK, GL_FILL);
            glPolygonMode(GL_FRONT, GL_FILL);
         }
         else
         {
            polygonMode=1;
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
      break;
      case 'd': //debug
         if(debug==true)
            debug=false;
         else
            debug=true;
      break;

      case 'l': //debug
         camera->lockPosition();
      break;

      case 's':
         {
		   int	StartTicks = glutGet(GLUT_ELAPSED_TIME);
		   int	ticks;
		   TriangleCounter = 0;
		   int	FrameCounter = 0;
		   int	TrisPerFrame = 0;

		   // For approximately one second, render frames as fast as we can.
		   for (;;) {
			   display();
			   FrameCounter++;
			   if (FrameCounter == 1) TrisPerFrame = TriangleCounter;
			   
			   ticks = glutGet(GLUT_ELAPSED_TIME);
			   if (ticks - StartTicks > 1000) break;
		   }

		   // Show the fps and tps results.
		   float	dt = (ticks - StartTicks) / 1000.0;
		   printf("\nRendered %0.1f frames/sec, %d tris/frame, %d tris/sec\n", FrameCounter / dt, TrisPerFrame, int(TriangleCounter / dt));
         }
	   break;

      case 't': //texture
         if(texture==true)
            texture=false;
         else
            texture=true;
      break;
      case 'c': //texture
         if(cullingB==true)
            cullingB=false;
         else
            cullingB=true;
      break;
   }
}  

//funcao para leitura do mouse e rotacao de toda cena. 
void mouseFunc(int button, int state, int x, int y)
{
   //printf("\nclicou e moveu mouse");
}



void specialFunc(int key, int x, int y)
{
	//printf("%c", key);
   //   printf("\n ");
	switch(key)
	{
		case 'd':  //seta esquerda
         camera->move(-200);
		break;
		case 'f': //seta direita
         camera->move(200);
		break;
	
      case 'h': ////pg up
         camera->elevate(200);
		break;
		case 'i': ////pg down
         camera->elevate(-200);
		break;

		case 'e': //seta cima
         camera->walk(200);
		break;
		case 'g': ////seta baixo
         camera->walk(-200);
		break;
	}
}

//retorna parametro [0,1] da posicao do mouse na tela
void passiveMotionFunc(int x, int y)
{
   oldX = x;
   oldY = y;
   max_render = (x+y);
   printf("\nMax_render %d", max_render);
}

void MotionFunc(int x, int y)
{
   float rx, rz;
   rx = x-oldX;
   rz = y-oldY;

   printf("\n%f %f", rx, rz);

   rx = rx/100.0;
   rz = rz/100.0;

   rx = x/100.0;
   rz = y/100.0;



   camera->rotate(rx, rz, 0);
   oldX = x;
   oldY = y;

}

////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   //glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutInitWindowPosition (350, 100);
   /* create window */
   glutCreateWindow ("Terrain pozzer v. 1.3");
   init();

   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialFunc);
   glutPassiveMotionFunc(passiveMotionFunc);
   glutMotionFunc(MotionFunc);
   glutIdleFunc(display);

   //int i=64;
   //printf("%d %d", i>>1, i>>2); 

   /* interact " */
   glutMainLoop();
   return 0;
}