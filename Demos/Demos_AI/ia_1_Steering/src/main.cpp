
#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/glut.h>
#include <stdio.h>

#include "global.h"
#include "Vector2.h"
#include "utils.h"
#include "Actor.h"
#include <windows.h>

Actor *actor;
Vector2 mousePos;
int steeringType;

void init()
{
	glClearColor(1,1,1,1);
	
	actor = new Actor();
	mousePos.set(300,300);
	steeringType = WANDER;
}

void reshape (int w, int h)
{
   //(x, y) --> (width, height)
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   //cria uma projecao ortografica com z entre (-1, 1). 
   //parametros: left, right, bottom, top, ou seja, (0,0) --> (largura,altura)
   gluOrtho2D (0.0, w, h, 0);
   glMatrixMode(GL_MODELVIEW);
}


void render()
{ 
   //posicao do mouse 
   glColor3d(1, 0, 0);
   glPushMatrix();
   glTranslated(mousePos.x, mousePos.y, 0);
   glBegin(GL_QUADS);
     glVertex2d(-5, -5);
     glVertex2d(-5, 5);
     glVertex2d(5, 5);
     glVertex2d(5, -5);
   glEnd();   
   glPopMatrix();
}

void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   Sleep(10); // para garantir um fps em torno de 100     
   
   actor->render();
   actor->update(0.01, mousePos, steeringType);
   render();

   glFlush();
   glutSwapBuffers();
}


void mouseMotionFunc(int x, int y)
{
   mousePos.x = x;
   mousePos.y = y;
}


void keyboard(unsigned char key, int x, int y)
{
   printf("\nTecla: %d" , key);
   switch(key)
   {
      case 27: //tecla esc
	     exit(0);
         break;

      case 'a': 
         steeringType = ARRIVE;
         break;

      case 's': 
         steeringType = SEEK;
         break;

      case 'f': 
         steeringType = FLEE;
         break;

      case 'w': 
         steeringType = WANDER;
         break;
   }
}	


int main (int argc, char* argv[])
{
   //teste da intersecao entre retas
   
   /*Vector2 v1(0,0), v2(4,4);     
   Vector2 p1(0,4), p2(4,0);     
   Vector2 inter;
   double dist;
   
   LineIntersection2D(v1, v2,    p1, p2,   dist, inter);
   inter.print();
   printf("\n Distancia =  %f", dist);
   getchar();
   /**/
   
   
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (250, 100);
   glutCreateWindow ("programa de teste");
    
   init();
    
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(mouseMotionFunc);
   
   glutIdleFunc(display);
   glutMainLoop();
    
   return 0;
}
