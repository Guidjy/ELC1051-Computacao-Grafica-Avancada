/**
*   Programa para ilustrar:
*   - Comparativo entre rotacao com angulos de Euler e quaternions
*   - Gimbal Lock
*   - rotacao em eixo arbitrario

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 06/10/2013
*   Adaptado de http://www.gamedev.net/reference/articles/article1095.asp
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glut.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "global.h"

#define SCREEN_X 900
#define SCREEN_Y 600

#define STEP 9

//global variables

//angulos de Euler
int yaw    =0, roll    =0, pitch    =0;
int old_yaw=0, old_roll=0, old_pitch=0;
int d_yaw  =0, d_roll  =0, d_pitch  =0;   //delta da variacao de angulo entre quadros


Quaternion g_quat_1, g_quat_2, g_quat_3, g_quat_4;
Matrix g_mat;
float g_angle=0;

void init()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(67.0, (SCREEN_X/3)/(SCREEN_Y/2), 0.1f, 100.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   Sleep(10);

   gluLookAt(0, 0, 10, //from. Posicao onde a camera esta posicionada
             0, 0, 0,  //to. Posicao absoluta onde a camera esta vendo
             0, 1, 0); //up. Vetor Up.

   //quadrantes
   //-------------------
   //|  1  |  2  |  3  |
   //-------------------
   //|  4  |  5  |  6  |
   //-------------------

   //quadrante 1
   glViewport (0, SCREEN_Y/2, SCREEN_X/3, SCREEN_Y/2);
   euler();

   //quadrante 2
   glViewport (SCREEN_X/3, SCREEN_Y/2, SCREEN_X/3, SCREEN_Y/2);
   quaternion_1();

   //quadrante 4
   glViewport (0, 0, SCREEN_X/3, SCREEN_Y/2);
   quaternion_2();

   //quadrante 5
   glViewport (SCREEN_X/3, 0, SCREEN_X/3, SCREEN_Y/2);
   eixo_arbitrario_euler();

   //quadrante 6
   glViewport ((SCREEN_X/3)*2, 0, SCREEN_X/3, SCREEN_Y/2);
   eixo_arbitrario_quat();


   glViewport (0,0, SCREEN_X, SCREEN_Y);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D (0.0, SCREEN_X, SCREEN_Y, 0.0 );
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();

 //-------------------------------------------------------------------------------------
 //imprime as legendas na tela
 //-------------------------------------------------------------------------------------
   char v[100];
   glColor3f(0, 0, 1);
   sprintf(v,"Pitch (x): %d  (up/down)",pitch);
   text(600,20, v);
   glColor3f(0, 0.5, 0.5);
   sprintf(v,"Yaw   (y): %d (left/right)",yaw);
   text(600,40, v);
   glColor3f(1, 0, 0);
   sprintf(v,"Roll  (z): %d (pg up/down)",roll);
   text(600,60, v);

   glColor3f(1, 0, 1);
   text(100,16,  "Angulos de Euler");
   text(350,16,  "Quaternio from Euler");
   text(50,316,  "Quaternio passo a passo");
   text(350,316, "Eixo arbitrario (Euler)");
   text(630,316, "Eixo arbitrario (Quaternio)");

   glutSwapBuffers();

   d_yaw = d_roll = d_pitch = 0;
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

      case 'r':
         yaw    =roll    =pitch    =0;
         old_yaw=old_roll=old_pitch=0;
         d_yaw  =d_roll  =d_pitch  =0;
         g_angle = 0;

         g_quat_1.identity();
         g_quat_2.identity();
         g_quat_3.identity();
         g_quat_4.identity();
         g_mat.identity();
      break;
      case '+':
         g_angle+=2;
      break;
      case '-':
         g_angle-=2;
      break;
   }
}

void specialFunc(int key, int x, int y)
{
   //printf("%d", key);
   switch(key)
   {
      case 101: //UP
         pitch += STEP;
         if (pitch >= 360)
             pitch -= 360;
      break;

      case 103: //down
         pitch -= STEP;
         if (pitch < 0)
             pitch += 360;
      break;

      case 100: //left
         yaw -= STEP;
         if (yaw < 0)
             yaw += 360;
      break;

      case 102: //right
         yaw += STEP;
         //pitch += STEP;
         if (yaw >= 360)
             yaw -= 360;
      break;

      case 105: //pgdown
         roll -= STEP;
         if (roll < 0)
             roll += 360;
      break;

      case 104: //pgup
         roll += STEP;
         if (roll >= 360)
             roll -= 360;
      break;
   }

   d_yaw   = yaw   - old_yaw;
   d_roll  = roll  - old_roll;
   d_pitch = pitch - old_pitch;

   old_yaw   = yaw;
   old_roll  = roll;
   old_pitch = pitch;
}

////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char**argv)
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutInitWindowPosition (50, 10);

   glutCreateWindow ("Euler x Quaternions - arrows, pgup, pgdown, +, -");
   init();

   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialFunc);

   glutMainLoop();
   return 0;
}
