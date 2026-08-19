#include "Actor.h"

Actor::Actor()
{
   massa       = 0.4;
   timeElapsed = 0;
   maxVel      = 150;
   maxForca    = 2;
   
   pos.set(200,200);
   vel.set(0,0);
   steeringForce.set(0,0);

   steering = new Steering();
}

void Actor::update(double time_elapsed, Vector2 targetPos, int steeringType)
{    
   timeElapsed = time_elapsed;      
   
   steeringForce = steering->calculateSteering(this, targetPos, steeringType);
 
   Vector2 acceleration = steeringForce / massa;

   //update velocity
   vel += (acceleration * time_elapsed); 

   //make sure vehicle does not exceed maximum velocity
   vel.truncate(maxVel);

   //update the position
   pos += vel * time_elapsed;

   //update the heading if the vehicle has a non zero velocity
   if (vel.lenghtSqr() > 0.00000001)
   {    
      Vector2 tmp(vel);   
      tmp.normalize();
      dir  = tmp;
      lado = dir.perp();
   }
}


//**************************************************************************************************   
//**************************************************************************************************   
//**************************************************************************************************   


void Actor::render()
{
   //renderiza o steering behavior
   steering->render(this);

   glPushMatrix();
   glLoadIdentity();

   //desenha o vetor direcao
   glColor3d(0, 1, 0);
   glTranslatef(pos.x, pos.y, 0);
   glBegin(GL_LINES);
      glVertex2d(0, 0);
      glVertex2d(dir.x*30, dir.y*30);
   glEnd();   
  
   //desenha o vetor forca
   glColor3f(0,0,1);
   glBegin(GL_LINES);
      glVertex2d(0, 0);
      glVertex2d(steeringForce.x, steeringForce.y);
   glEnd();     

   //calcula o angulo do personagem
   Vector2 vet(0,1);
   float ang = dir.angle(vet);
   ang *= (180/3.14);

   //desenha o actor
   glColor3d(0, 1, 0);
   glRotatef(-ang, 0, 0, 1);
   glBegin(GL_TRIANGLES);
      glVertex2d(-5, -5);
      glVertex2d(5, -5);
      glVertex2d(0, 15);
   glEnd();   

   glPopMatrix();
}
