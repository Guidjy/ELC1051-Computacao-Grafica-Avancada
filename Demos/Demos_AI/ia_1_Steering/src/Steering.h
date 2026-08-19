
#ifndef __STEERING_H
#define __STEERING_H

#include "Vector2.h"
#include "global.h"
#include "utils.h"

class Actor;

class Steering
{
   //usado no comportametno Wander
   Vector2     wanderTarget; 
   float       wanderJitter;
   float       wanderRadius;
   float       wanderDistance;
   
   //usado no comportamento WallAvoidance
   Vector2 antenas[3];
   float   wallDetectionFeelerLength; //comprimento das antenas
   
   //definicao de um muro: 0 e 1 definem o ponto e 2 o vetor normal do muro
   Vector2 wall[3];
   
   int steeringType;

public:
   Steering();

   Vector2 calculateSteering(Actor *actor, Vector2 targetPos, int steeringType);
   
   void render(Actor *actor); 

private:
   Vector2 seek(Actor *actor, Vector2 targetPos);
   
   Vector2 flee(Actor *actor, Vector2 targetPos);
   
   Vector2 arrive(Actor *actor, Vector2 targetPos);

   Vector2 wander(Actor *actor);

   Vector2 wallAvoidance(Actor *actor);
   
   
   //********************************
   float randomClamped();
   void drawCircle(float raio);
   void createFeelers(Actor *actor);
};


#endif
