//----------------------------------------------------------
// Classe para definicao de um personagem 3D em formato MD2
// pozzer@inf.ufsm.br
//----------------------------------------------------------

#ifndef __ACTOR___H__
#define __ACTOR___H__

#include <GL/glut.h>
#include <stdio.h>

#include "Vector3.h"
#include "md2.h"


class Actor
{
   Md2 * body,   //body model
       * weapon; //weapon model. e.g. sword.

   float  actorScale;
   float  frameInterp;   // facteur d'interpolation
   int    frameCounter;   // contador de frames executados em cada acao
   int    frameCurr;      // frame courante
   int    frameStart, frameEnd; //indices de inicio e fim da animacao. Depende da acao.

   float rotacao;
public:
   Actor()
   {
      body = weapon = NULL;

      actorScale = 1;

      frameInterp = 0;  // facteur d'interpolation
      frameCounter;     // contador de frames executados em cada acao
      frameCurr = 0;    // frame courante
      frameStart = 0;
      frameEnd = 190;   //indices de inicio e fim da animacao. Depende da acao.

      rotacao = 0;
   }

   void loadModel(const char *path, const char *model, const char *texture)
   {
      body   = new Md2();
      body->load(path, model, texture );
   }

   void loadWeapon(const char *path, const char *model, const char *texture)
   {
      weapon = new Md2();
      weapon->load(path, model, texture );
   }

   Vector3 getBoundingBox(int i)
   {
      return body->getBoundingBox(i);
   }

   void scale(float f)
   {
      actorScale = f;
      if( body != NULL )
         body->scale(f);
      if( weapon != NULL )
         weapon->scale(f);
   }

   void render()
   {
      if( body != NULL  )
      {
         body->Animate(frameStart, frameEnd, 0.01f, &frameInterp, &frameCurr, true);
         weapon->Animate(frameStart, frameEnd, 0.01f, &frameInterp, &frameCurr, true);
         printf("\r%d", frameCurr);
      }
      if( weapon != NULL )
      {
          body->Animate(frameStart, frameEnd, 0.01f, &frameInterp, &frameCurr, true);
         weapon->Animate(frameStart, frameEnd, 0.01f, &frameInterp, &frameCurr, true);
      }
   }
};

#endif
