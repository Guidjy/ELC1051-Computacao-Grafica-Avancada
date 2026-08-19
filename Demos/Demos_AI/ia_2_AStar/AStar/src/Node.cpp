#include "Node.h"
#include <math.h>

Node::Node(int x, int y, Node *parent, float g)
{
   this->x = x;
   this->y = y;
   this->parent = parent;
   this->g = g;
   f = 0;
   h = 0;
}

Node::Node(int x, int y)
{
   this->x = x;
   this->y = y;
   this->parent = NULL;
   this->g = 0;
   f = 0;
   h = 0;
}

Node::Node(Node *n)
{
   this->x = n->x;
   this->y = n->y;
   this->parent = n->parent;
   this->g = n->g;
   this->f = n->f;
   this->h = n->h;
}

void Node::estimate(Node *target)
{
   //calcula a distancia do no ao destino
   if( target != NULL ) //A*
      h = (float)sqrt( (double)((target->x - x)*(target->x - x) + (target->y - y)*(target->y - y)) );
   else         //Dijkstra
      h = 0;
   f = g + h;
}
   
bool Node::equals(Node *n)
{
   if( n == NULL || x != n->x || y != n->y)
      return false;
   return true;
}
