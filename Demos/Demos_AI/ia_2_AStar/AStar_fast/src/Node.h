//--------------------------------------------------
//  Autor: Cesar Tadeu Pozzer
//  17/07/2007
//  pozzer3@gmail.com
//--------------------------------------------------


#ifndef ___NODE_H__
#define ___NODE_H__

#include <math.h>
#include <stdlib.h>

class Node  
{
public:
   int x, y;      //coordenadas do grafo
   float f, g, h; //usados pelo A* 
   Node *parent;
   Node *next;
   Node *prev;

   Node(int x, int y, Node *parent, float g);

   Node(int x, int y);

   Node(Node *n);

   void estimate(Node *target);
   
   bool equals(Node *n);
};

#endif 
