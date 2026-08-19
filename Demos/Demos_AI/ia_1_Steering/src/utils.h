// Vector3.h: interface for the Vector3 class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __UTILS_H
#define __UTILS_H

#include "Vector2.h"


//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs.
//
//-----------------------------------------------------------------
inline bool LineIntersection2D(Vector2 A,
                               Vector2 B,
                               Vector2 C, 
                               Vector2 D)
{
   double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
   double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

   double Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

   if (Bot == 0)//parallel
   {
      return false;
   }

   double invBot = 1.0/Bot;
   double r = rTop * invBot;
   double s = sTop * invBot;

   if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
   {
      //lines intersect
      return true;
   }

   //lines do not intersect
   return false;
}



//-------------------- LineIntersection2D-------------------------
//
// Given 2 lines in 2D space AB, CD this returns true if an 
// intersection occurs and sets dist to the distance the intersection
// occurs along AB. Also sets the 2d vector point to the point of
// intersection
//----------------------------------------------------------------- 
inline bool LineIntersection2D(Vector2   A,
                               Vector2   B,
                               Vector2   C, 
                               Vector2   D,
                               double&   dist,
                               Vector2&  point)
{

   double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
   double rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

   double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
   double sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

   if ( (rBot == 0) || (sBot == 0))
   {
      //lines are parallel
      return false;
   }
   double r = rTop/rBot;
   double s = sTop/sBot;

   if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
   {
      dist = A.distance(B) * r;
      point = A + (B - A) * r;

      return true;
   }
   else
   {
      dist = 0;
      return false;
   }
}


#endif
