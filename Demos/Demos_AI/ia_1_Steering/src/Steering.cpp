

#include "Actor.h"
#include "Steering.h"
#include <math.h>


Steering::Steering()
{
   wanderJitter   = 80.0;
   wanderRadius   = 30.0;
   wanderDistance = 40.0;

   wallDetectionFeelerLength = 100;

   //create a vector to a target position on the wander circle
   double theta = -0.5; //valor aleatorio
   wanderTarget.set(wanderRadius * cos(theta),  wanderRadius * sin(theta));

   //define o muro
   wall[0].set(200,200);
   wall[1].set(300,300);
   Vector2 t(wall[0]-wall[1]);
   t = t.perp();
   t.normalize();
   wall[2].set(t); //vetor perpendicular
}

Vector2 Steering::calculateSteering(Actor *actor, Vector2 targetPos, int type)
{
   Vector2 wallForce = wallAvoidance(actor);
   if( wallForce.lenghtSqr() > 1 )
   {
      return wallForce;
   }

   steeringType = type;
   switch(steeringType)
   {
       case SEEK:
           return seek(actor, targetPos);
           break;

       case FLEE:
           return flee(actor, targetPos);
           break;

       case ARRIVE:
           return arrive(actor, targetPos);
           break;

       case WANDER:
           return wander(actor);
           break;
   }
}

Vector2 Steering::seek(Actor *actor, Vector2 targetPos)
{
   Vector2 desiredVelocity;
   desiredVelocity = targetPos - actor->pos;
   desiredVelocity.normalize();
   desiredVelocity *= actor->maxVel;

   return (desiredVelocity - actor->vel);
}


Vector2 Steering::flee(Actor *actor, Vector2 targetPos)
{
   double panicDistance = 100;
   if( actor->pos.distance(targetPos) > panicDistance )
   {
     return Vector2(0,0);
   }
   Vector2 desiredVelocity;
   desiredVelocity = actor->pos - targetPos;
   desiredVelocity.normalize();
   desiredVelocity *= actor->maxVel;

   return (desiredVelocity - actor->vel);
}

Vector2 Steering::arrive(Actor *actor, Vector2 targetPos)
{
   Vector2 toTarget = targetPos - actor->pos;

   //calculate the distance to the target
   double dist = toTarget.lenghtSqr();
   float deceleration = 1;

   if(dist > 0)
   {
      //calculate the speed required to reach the target given the desired
      //deceleration
      double speed = dist / deceleration;

      //make sure the velocity does not exceed the max
      speed = speed < actor->maxVel ? speed : actor->maxVel;

      //from here proceed just like Seek except we don't need to normalize
      //the ToTarget vector because we have already gone to the trouble
      //of calculating its length: dist.
      Vector2 desiredVelocity = toTarget * speed / dist;
      desiredVelocity.print();

      return (desiredVelocity - actor->vel);
   }
   return Vector2(0,0);
}


//--------------------------- Wander -------------------------------------
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vector2 Steering::wander(Actor *actor)
{
   //this behavior is dependent on the update rate, so this line must
   //be included when using time independent framerate.
   double JitterThisTimeSlice = wanderJitter * actor->timeElapsed * 9;

   Vector2 tmp(randomClamped() * JitterThisTimeSlice,
               randomClamped() * JitterThisTimeSlice);

   printf("\n\n J= %lf", JitterThisTimeSlice);
   wanderTarget.print();
   tmp.print();

   //first, add a small random vector to the target's position
   wanderTarget += tmp;

   //reproject this new vector back on to a unit circle
   wanderTarget.normalize();

   //increase the length of the vector to the same as the radius
   //of the wander circle
   wanderTarget *= wanderRadius;
   wanderTarget.print();

   //Pozzer
   Vector2 target(actor->pos);
   target += (actor->dir*wanderDistance);
   target += wanderTarget;

   return target - actor->pos;
}


Vector2 Steering::wallAvoidance(Actor *actor)
{
   createFeelers(actor);

   double distance = 0.0;

   Vector2 SteeringForce,
           point;         //used for storing temporary info

   //examine each feeler in turn
   for (int i=0; i<3; i++)
   {
      if (LineIntersection2D(actor->pos, antenas[i], wall[0], wall[1], distance, point))
      {
         //calculate by what distance the projected position of the agent
         //will overshoot the wall
         Vector2 OverShoot = antenas[i] - point;

         //create a force in the direction of the wall normal, with a
         //magnitude of the overshoot
         //normal[2] eh a normal da parede definida por [ wall[0], wall[1] ]
         SteeringForce = wall[2] * OverShoot.lenghtSqr();

         //determina a direcao da forca em funcao da posicao relativa do muro e do actor
         Vector2 v1(wall[0] - wall[1]);
         Vector2 v2(wall[0] - actor->pos);
         int side = v1.side(v2);

         //printf("\nColidiu lado %d antena %d ", side, i);
         if( side == -1 )
            return SteeringForce;
         else
            return SteeringForce.reverse();
     }
  }//next feeler

  return SteeringForce;

}


//*************************************************************
void Steering::render(Actor *actor)
{
   Vector2 target(actor->pos);
   target += (actor->dir*wanderDistance);

   Vector2 wTarget(target);
   wTarget+=wanderTarget;

   if( steeringType == WANDER )
   {
      //desenha o circulo na frente do actor
      glPushMatrix();
      glColor3f(1,0,0);
      glTranslated(target.x, target.y, 0);
      drawCircle(wanderRadius);
      glPopMatrix();

      //desenha o target
      glPushMatrix();
      glTranslated(wTarget.x, wTarget.y, 0);
      drawCircle(4);
      glPopMatrix();
   }

   //desenha o muro
   glColor3f(0,1,0);
   glPushMatrix();
   glLoadIdentity();
   glBegin(GL_LINES);
      glVertex2d(wall[0].x, wall[0].y);
      glVertex2d(wall[1].x, wall[1].y);
   glEnd();

   //desenha as antenas
   glColor3f(1,0,0);
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(actor->pos.x, actor->pos.y, 0);
   glBegin(GL_LINES);
      for(int i=0; i<3; i++)
      {
         Vector2 tmp(antenas[i]);
         tmp -= actor->pos;
         glVertex2d(0, 0);
         glVertex2d(tmp.x, tmp.y);
      }
   glEnd();
   glPopMatrix();
}

void Steering::drawCircle(float raio)
{
   float x, y;
   float ang = 0;
   glBegin(GL_LINE_STRIP);
   for(int i=0; i<50; i++)
   {
       x = cos(ang)*raio;
       y = sin(ang)*raio;
       glVertex2f(x, y);

       ang += (2*3.14)/49;
   }
   glEnd();
}

//retorna valor entre [-1, 1]
float Steering::randomClamped()
{
   float r = 1.0 - ((rand()%1000)/500.0);
   printf("\nR= %.2f", r);
   return r;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void Steering::createFeelers(Actor *actor)
{
  //feeler pointing straight in front
  antenas[0] = actor->pos + (actor->dir * wallDetectionFeelerLength);

  //feeler to left
  Vector2 temp = actor->dir;
  temp.rotate(3.14/4.0); //45 graus
  antenas[1] = actor->pos + (temp*wallDetectionFeelerLength/2.0f);

  //feeler to right
  temp = actor->dir;
  temp.rotate(-3.14/4.0); //45 graus
  antenas[2] = actor->pos + (temp*wallDetectionFeelerLength/2.0f);
}
