#ifndef __CIRCLE___H__
#define __CIRCLE___H__

#define PI_2 6.28318530717958
#define DIV  30

class Circle
{
public:
   Vector p;//position (p) and radius (r). 
   float  r;  

   Circle(Vector _p, float _r)
   {
      p = _p;
      r = _r;
   }   
   
   inline void setPos(Vector _p)
   {
      p = _p;          
   }

   inline void setRadius(float _r)
   {
      r = _r;
   }
 
   void render()
   {
      float x1, y1;
      float inc = PI_2/DIV;
      glBegin(GL_LINE_LOOP);
         for(float ang=0; ang<PI_2; ang+=inc)
         {
            x1 = (int)(cos(ang)*r);
            y1 = (int)(sin(ang)*r);
            glVertex2d(x1+p.x, y1+p.y);
         }
      glEnd();             
   }
};

#endif
