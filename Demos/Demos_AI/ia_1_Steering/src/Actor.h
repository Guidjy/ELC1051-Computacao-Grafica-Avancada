// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <math.h>
#include <stdio.h>
#include "Steering.h"
#include <gl/glut.h>


//OBS: por questoes de simplicidade, todos os atributos sao publicos. 
class Actor 
{

public:
   Vector2 pos,  //posicao
           dir,  //direcao
           lado, //vetor perperndicular a direcao
           vel;  //velocidade

   Vector2 steeringForce;

  
   float   raio;
   double  massa;        //massa do actor
   float   timeElapsed;
   double  maxVel;       //velocidade maxima
   double  maxForca;     //forca maxima que o actor pode produzir
   
   Steering *steering;
   
   Actor();
   
   void update(double time_elapsed, Vector2 targetPos, int steeringType);

   void render();
};

#endif
