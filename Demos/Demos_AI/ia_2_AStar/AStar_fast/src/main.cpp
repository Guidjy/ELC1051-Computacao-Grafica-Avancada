#include <stdio.h>

#include "AStar.h"
#include "List.h"
#include "Graph.h"


//matriz de custos. Valores =9 representam paredes e nao podem ser ultrapassados
int c[LARG][ALT]={0, 0, 0, 0, 0, 0, 0, 9, 0, 0,
                  0, 9, 0, 9, 0, 9, 0, 0, 0, 0,
                  6, 9, 7, 9, 9, 9, 9, 9, 9, 0,
                  0, 0, 0, 0, 0, 0, 0, 9, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 9, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 9, 0, 0,
                  0, 9, 9, 9, 9, 9, 0, 9, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 9, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 9, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 9, 0, 0};


int main(void)
{
   AStar *astar;
   Graph *graph;
   List	 *path;

   graph = new Graph(c);
   astar = new AStar(graph);

   path = astar->search(4, 8, 8, 4);  //BOM
   astar->printVisited();
   graph->print(path);
   path->print();

   path->clear();
   path = astar->buildPath(0, 4);  //BOM
   graph->print(path);
   path->print();

   path->clear();
   path = astar->buildPath(9, 0);  //Nao Tem Caminho
   graph->print(path);
   path->print();

/*
   astar->printVisited();
   path->clear();
   path = astar->search(1, 2);
   path->print(); //Nao imprime nada pois eh Dijkstra
   path = astar->buildPath(6,6);
   astar->printVisited();
   path->print();
   graph->print(path);
   //path->print();

/**/
   getchar();  
   return 0; 
}
