
#include "AStar.h"
#include "Graph.h"
#include "Node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

List* AStar::search(int startx, int starty)
{
   Node *startNode  = new Node(startx, starty);
   Node *targetNode = NULL;
   return search( startNode, targetNode );
}

List* AStar::search(int startx, int starty, int targetx, int targety)
{
   Node *startNode  = new Node(startx, starty);
   Node *targetNode = new Node(targetx, targety);
   return search( startNode, targetNode );
}


//metodo privado.
//List* AStar::search(Node *startNode, int goalx, int goaly)
List* AStar::search(Node *startNode, Node *targetNode )
{
   //testa os limites da busca em relacao ao tamanho do grafo
   testLimits( startNode, targetNode );

   //para permitir uma busca a um no em tempo O(1)
   Node *indexOpen[width][height];
   //Node *indexClosed[width][height];

   //inicializa indexOpen e indexClosed para NULL
   memset(indexOpen,   0, width*height*sizeof(Node*));
   memset(indexClosed, 0, width*height*sizeof(Node*));

   //startNode->estimate(goalx, goaly);
   startNode->estimate(targetNode);
    
   open->insert(startNode, false);
   indexOpen[startNode->x][startNode->y] = open->prim;
	
   //enquanto tiver elementos da lista de nos ABERTOS	
   while( ! open->isEmpty()  )
   {
      //encontra e remove o MELHOR_NO de ABERTOS 
      Node *node = open->getMinF();
	  indexOpen[node->x][node->y] = NULL;

      //coloca MELHOR_NO de ABERTOS em FECHADOS
	  indexClosed[node->x][node->y] = node;
	  closed->insert(node, false);

      //testa se MELHOR_NO ja nao eh o destino
      if( node->equals(targetNode) )
      {
         List* path = buildPath(node);
         return path;
      }
      else
      {
         bool inOpen, inClosed;
         Node *nodeVisited;
         Node *tmp, *novo;

         // Construct the list of nodes
         sucessors->clear();
         buildSucessors(node, 5);

         // For each node in suscessors
         for (tmp = sucessors->prim; tmp!=NULL; tmp=tmp->next)
         {
            // Test if sucessor is in Open or Closed
            inOpen = false; inClosed = false;
            nodeVisited = NULL;

            if(indexOpen[tmp->x][tmp->y] != NULL)
            {
               inOpen = true;
               nodeVisited = indexOpen[tmp->x][tmp->y];
            }
            if(indexClosed[tmp->x][tmp->y] != NULL)
            {
               inClosed = true;
               nodeVisited = indexClosed[tmp->x][tmp->y];
            }
		
            //se o no nao esta em [ABERTO, FECHADO] ou custoG > novoVertice, o novo caminho
            //encontrado eh melhor que o antigo e deve substitui-lo
            //if( (!inOpen && !inClosed) || nodeVisited->getG() > tmp->getG() )
            if( nodeVisited==NULL || nodeVisited->g > tmp->g )
            {
               // If node exists, remove it from ABERTO or FECHADO
               if (inOpen)
               {
                  indexOpen[nodeVisited->x][nodeVisited->y]=NULL;
                  open->remove(nodeVisited, true);
               }
               if (inClosed)
               {
                  indexClosed[nodeVisited->x][nodeVisited->y]=NULL;
                  closed->remove(nodeVisited, true);
               }
               // Calculate estimated cost to goal
               tmp->estimate(targetNode);
					
               novo = open->insert(tmp, true);
               indexOpen[tmp->x][tmp->y]=novo;
            }
         } // for (list of sucessors)
         sucessors->clear();
      } // if (goal)
   } // while
   
   //retorna uma lista vazia pois nao achou o caminho
   return new List();
}

void AStar::printVisited()
{
   //imprime os nos visitados:
   for(int i=0; i<10; i++) 
   {
     printf("\n");     
     for(int j=0; j<10; j++)
     if( indexClosed[i][j] != NULL )
        printf(" .");
     else
        printf("  ");
   }
}

//Retorna em List a lista com os N nós vizinhos de node, onde N = neighbors. 
//Se N for 8, todos os nos vizinhos sao consultados. Senao, somente os mais a frente do caminho de busca
void AStar::buildSucessors(Node *node, int neighbors)
{
   Node *newNode;
   Node *parent = node->parent;
   //int x = node->x, y = node->y;
   int  px, py;
   int ini=0, fim=8, i;
   int pos[14][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} };

   if( parent != NULL && neighbors < 8 )
   {
      //acha a posicao do no pai relativao ao no node. 
      for(i=0; i<8; i++)
         if(pos[i][0]==(parent->x - node->x) && pos[i][1]==(parent->y - node->y))
            break;
      if(neighbors==5)
         ini = i+2;
      else if(neighbors==3)
         ini = i+3;
      fim = ini + neighbors;
   }
   for(i=ini; i<fim; i++)
   {
      px = node->x + pos[i][0];
      py = node->y + pos[i][1];
	  assert(i<14);
	     
      if( px >= 0 && py >=0 && px < width && py < height )
      {
   	     //verifica se nao tem uma parede na frente do caminho
         if( graph->isWall(px, py) )
   	     {
            continue;
         }  
         float g = node->g + graph->getCost(node->x, node->y, px, py);
         //todos os sucessores apontam para o pai, para poder reconstruir o caminho
         newNode = new Node(px, py, node, g ); 
         //printf("\nsucessor de (%d %d)  ->  %d %d = %.1f", node->x, node->y, px, py, g);
         sucessors->insert(newNode, false);
      }
   }
}

List * AStar::buildPath(int x, int y)
{
     Node *node = indexClosed[x][y];
     return buildPath(node);
}

//cria uma lista que contem o path requisitado. Nao destroi a STP gerada ateh entao. 
List * AStar::buildPath(Node *node)
{
   List *path = new List();
   while (node != NULL)
   {
      path->insert(node, true);
      node = node->parent;
   }
   return path;
}

//verifica se os parametros de busca estao dentro da area do grafo
void AStar::testLimits(Node *startNode, Node *targetNode)
{
   if( startNode->x > width || startNode->x < 0 || startNode->y > height || startNode->y < 0 )
   {
      printf("No de origem fora dos limites do grafo");
      getchar();
      exit(1);
   }
   if( targetNode != NULL)
   if( targetNode->x > width || targetNode->x < 0 || targetNode->y > height || targetNode->y < 0 )
   {
      printf("No de destino fora dos limites do grafo");
      getchar();
      exit(1);
   }
}

AStar::AStar(Graph *g)
{
   this->graph = g;
   width  = g->getWidth();
   height = g->getHeight();

   open      = new List();  //lista de nos em aberto 
   closed    = new List();  //lista de nos processados
   sucessors = new List();  //lista de nos sucessores de um dado no. Ver buildSucessors()
}
