//*********************************************************************
//
// Programa para ilustrar uso de um grafo de cena
//
// Autor: Cesar Tadeu Pozzer
// pozzer@inf.ufms.br
// 2007/2
//
//*********************************************************************

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "frames.h"

#include "node.h"
#include "appearance.h"
#include "shape.h"
#include "cubo.h"
#include "cilindro.h"
#include "group.h"
#include "scene.h"
#include "transform.h"
#include "lightsource.h"
#include "camera.h"
#include "esfera.h"
#include "Bmp.h"

#define PERNA 0
#define TAMPO 1


Frames   f;
int posicao;
double rx, ry, rz;
double tx, ty, tz;

int polygonMode = 1;
int cullingMode = 1;
int frontfaceMode = 1;

scene       *sc;
plight      *luz1;
spotlight   *spot;

perspective *camera;

transform   *tr_mesa[5];  //posicao da mesa, pernas e tampo
transform   *tr_retrato[3];
transform   *tr_spot;
transform   *tr_cone;

material    *mt_cena;

cilindro    *cl_cone, *cl_retrato;

texture     *tt_mesa, *tt_retrato, *tt_chao;

cubo        *sala;
cubo        *cb_mesa[2];  //perna(s) e tampo da mesa
cubo        *cb_retrato;  //porta retrato


void gera_materiais()
{
   mt_cena = new material();
   mt_cena->SetAmbient(0.2, 0.2, 0.2, 1);
   mt_cena->SetDiffuse(0.3, 0.3, 0.7, 1);
   mt_cena->SetSpecular(0.4, 0.4, 0.4, 1);
}

//------------------------------------------------------------
//porta retrato
//------------------------------------------------------------
void gera_retrato()
{
   cl_retrato = new cilindro("cilindro do porta retrato");
   cl_retrato->setAltura(3);
   cl_retrato->setResolucao(11,2);
   cl_retrato->setRaios_1(0.01, 0.3);
   cl_retrato->setRaios_2(0.01, 0.3);
   tr_retrato[1] = new transform("tr do cilindro do retrato");
   tr_retrato[1]->setRotacao(35, 0, 0, 1);
   tr_retrato[1]->setTranslacao(1.1, -4, 0);
   tr_retrato[1]->add(cl_retrato);

   cb_retrato = new cubo("cubo do retrato");
   cb_retrato->setDimensoes(5, 0.2, 5);
   cb_retrato->setTextura(0x100000);
   cb_retrato->setResolucao(8,1,8,1);

   tr_retrato[2] = new transform("tr do cubo do retrato");
   tr_retrato[2]->setRotacao(60, 0, 0, 1);
   tr_retrato[2]->setTranslacao(0, -3, 0);
   tr_retrato[2]->add(cb_retrato);

   tr_retrato[0] = new transform("tr do retrato");
   tr_retrato[0]->setTranslacao(0, 5.8, -6);
   tr_retrato[0]->setRotacao(150, 0, 1, 0);
   tr_retrato[0]->add(mt_cena);
   tr_retrato[0]->add(tr_retrato[1]);
   tr_retrato[0]->add(tr_retrato[2]);
}


void gera_luminaria()
{
   spot = new spotlight("luz spot");
   spot->SetShiness(4);
   spot->SetRaio(0.5);
   spot->SetPosition(0.2, -0.2, 0.3);
   spot->SetApperture(62);
   spot->setOrientacao(0, -1, 0);
   spot->SetAmbient(1,1,1);
   spot->SetDiffuse(1,1,1);
   spot->SetSpecular(1,1,1);

   cl_cone = new cilindro("cone de luz");
   cl_cone->setAltura(2);
   cl_cone->setResolucao(11,2);
   cl_cone->setRaios_1(0,0.1);
   cl_cone->setRaios_2(1,1.1);

   tr_cone = new transform("transform cone");
   tr_cone->setTranslacao(0.0, 4.1, 0);
   tr_cone->setRotacao(-105, 0 ,0, 1);
   tr_cone->setAnimacao(6.75, 0.2, 1, 0.2);

   tr_cone->add(cl_cone);
   tr_cone->add( spot );
}


void gera_sala()
{
   sala = new cubo("Sala" );
   sala->setDimensoes(115,55,115);
   //sala->setResolucao(18, 14, 18, 5);
   sala->setResolucao(100, 100, 100, 5);
   sala->setNormal(true);
   //sala->setTextura(0x111111);
   sala->setTextura(0x111111);
}

void gera_mesa()
{
   cb_mesa[PERNA] = new cubo("pernas");
   cb_mesa[PERNA]->setDimensoes(1.3, 12, 1);
   cb_mesa[PERNA]->setResolucao(1,1,1, 0.9);
   cb_mesa[PERNA]->setTextura(0x111111);

   cb_mesa[TAMPO] = new cubo("tampo da mesa"); //pernas da mesa
   cb_mesa[TAMPO]->setDimensoes(22,1,22);
   cb_mesa[TAMPO]->setResolucao(48, 1, 48, 1);
   cb_mesa[TAMPO]->setTextura(0x101111);

   tr_mesa[1] = new transform("tranformacao perna 1");
   tr_mesa[1]->setTranslacao(-8, -6, 8);
   tr_mesa[1]->add(cb_mesa[PERNA]);

   tr_mesa[2] = new transform("tranformacao perna 2");
   tr_mesa[2]->setTranslacao(8, -6, 8);
   tr_mesa[2]->add(cb_mesa[PERNA]);

   tr_mesa[3] = new transform("tranformacao perna 3");
   tr_mesa[3]->setTranslacao(-8,-6, -8);
   tr_mesa[3]->add(cb_mesa[PERNA]);

   tr_mesa[4] = new transform("tranformacao perna 4");
   tr_mesa[4]->setTranslacao(8, -6, -8);
   tr_mesa[4]->add(cb_mesa[PERNA]);

   tr_mesa[0] = new transform("tranformacao de toda mesa");
   tr_mesa[0]->setTranslacao(0,-15,-30);
   tr_mesa[0]->setAnimacao(0.2, 0,1,0);

   tr_mesa[0]->add(mt_cena);
   tr_mesa[0]->add(cb_mesa[TAMPO]);
   tr_mesa[0]->add(tr_mesa[1]);
   tr_mesa[0]->add(tr_mesa[2]);
   tr_mesa[0]->add(tr_mesa[3]);
   tr_mesa[0]->add(tr_mesa[4]);

   tr_mesa[0]->add(tr_cone);

   tr_mesa[0]->add(tt_retrato );
   tr_mesa[0]->add(tr_retrato[0]);
}


void gera_luzes()
{
   luz1 = new plight();
   luz1->SetRaio(1);
   luz1->SetPosition(0, 16, 0);
   luz1->SetAmbient(0.8, 0.8, 0.5);
   luz1->SetDiffuse(0.8, 0.8, 0.5);
   luz1->SetSpecular(0.8, 0.8, 0.5);
   luz1->SetShiness(4);
}


//faz a geracao da cena. Esta funcao e' chamada apenas no inicio da execucao.
void init()
{
   rx, ry, rz, tx, ty = -0.0;
   tz = -60;

   glShadeModel(GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glEnable(GL_DEPTH_TEST);

   camera  = new perspective("perspectiva");

   tt_chao = new texture("textura do chao da sala");
   tt_retrato= new texture("textura do retrato");
   tt_mesa   = new texture("textura da mesa de madeira");

   Bmp *image1 = new Bmp("resources\\chao2.bmp");
   Bmp *image2 = new Bmp("resources\\foto.bmp");
   Bmp *image3 = new Bmp("resources\\mesa.bmp");

   //associa as imagens com as texturas
   tt_chao->bindTexture( image1 );
   tt_retrato->bindTexture( image2 );
   tt_mesa->bindTexture( image3 );

   //faz a geracao dos compoentes da cena



   gera_materiais();
   gera_luzes();

   gera_retrato();
   gera_luminaria();
   gera_mesa();
   gera_sala();

   //---------------------------------------------------
   //geracao da cena final
   //---------------------------------------------------

   sc = new scene();
   sc->add( luz1 );

   sc->add( mt_cena );
   sc->add( tt_chao );
   sc->add( sala   );

   sc->add( tt_mesa );
   sc->add( tr_mesa[0] );

   //sc->add( camera );

   glClearColor(1,1,1,1);
}


////////////////////////////////////////////////////////////////////////////////////////
void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(55,1,10,1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity( );
   gluLookAt(0,0,4, 0,0,0, 0,1,0);


   //faz o posicionamento da cena segundo vontade do usuario
   glTranslatef(tx, ty, tz);
   glRotated(rx, 1,0,0);
   glRotated(ry, 0,1,0);
   glRotated(rz, 0,0,1);

   glColor3f(1,0,0);
   //renderiza a cena
   sc->Render();
//glutSolidCube(100);


   //mostra a taxa de renderizacao da cena
   f.getFrames();
   glutSwapBuffers();

   Sleep(10);
}

void special(int key, int x, int y)
{
   //printf("%d", key);
}

//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   int k;
   k = key;
   //printf("%d", key);
   switch(k)
   {
      case 27:
         exit(0);
      break;

      case 53:
         ty+=1;
      break;

      case 50:
         ty-=1;
      break;

      case 49:
         tx-=1;
      break;

      case 51:
         tx+=1;
      break;

      case 43:
         tz+=1;
      break;

      case 45:
         tz-=1;
      break;

      case 119: //w = wireframe
         if(polygonMode==1)
         {
            polygonMode=0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
         else
         {
            polygonMode=1;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         }
      break;

      case 'c': //culling
         if( cullingMode == 1 )
         {
            cullingMode = 0;
            glEnable(GL_CULL_FACE);
         }
         else
         {
            cullingMode = 1;
            glDisable(GL_CULL_FACE);
         }
      break;

      case 'f': //front face
         if( frontfaceMode == 1 )
         {
            frontfaceMode = 0;
            glFrontFace(GL_CW);
         }
         else
         {
            frontfaceMode = 1;
            glFrontFace(GL_CCW);
         }
      break;

      case 108: //l = lighting
         luz1->setLuzLigada();
      break;

      case 115: //s = spot
         spot->setLuzLigada();
      break;
   }
}


//funcao para leitura do mouse e rotacao de toda cena.
void mouseFunc( int x, int y)
{
   rx = x;
   ry = y;
}


////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (650, 600);
   glutInitWindowPosition (250, 100);
   /* create window */
   glutCreateWindow ("Grafo de Cena. Use l, w, s, c, f, +, -, mouse");
   init();

   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMotionFunc(mouseFunc);
   glutIdleFunc(display);

   glutMainLoop();
   return 0;
}
