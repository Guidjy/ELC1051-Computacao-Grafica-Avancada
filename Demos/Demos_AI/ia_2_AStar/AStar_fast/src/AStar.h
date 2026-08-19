#ifndef _ASTAR_H__
#define _ASTAR_H__

#include "List.h"
#include "Graph.h"
#include "Node.h"


class AStar  
{
   List* open;       //lista de nos em aberto 
   List* closed;     //lista de nos processados
   List* sucessors;  //lista de nos sucessores de um dado no. Ver buildSucessors()
      
public:
   AStar(Graph *g);

   //faz a busca no grafo para gerar a SPT a partir de um ponto inicial 
   List* search(int startx, int starty);
   //faz a busca no grafo para gerar o caminho entre 2 pontos
   List* search(int startx, int starty, int targetx, int targety);
   
   //imprime os nos visitados pelo algoritmo A*
   void printVisited();
    
   //reconstroi o caminho do nó destino ao nó origem.
   List * buildPath(int x, int y);
   
private:
   Graph *graph;
   int width, height; //dimensoes do grafo de busca. 

   Node *indexClosed[LARG][ALT]; //matriz de indices para o vetor CLOSED

   List* search(Node *startNode, Node *targetNode);
	
   //reconstroi o caminho do nó destino ao nó origem.
   List *buildPath(Node *node);

   //metodo que depende da forma como o grafo esta organizado. Neste exemplo, utiliza-se
   //uma matriz retangular onde cada no pode ter ateh 8 vizinhos. 
   void buildSucessors(Node *node, int neighbors);
	
   //verifica se os parametros de busca estao dentro da area do grafo
   void testLimits(Node *startNode, Node *targetNode);
   
};

#endif 
