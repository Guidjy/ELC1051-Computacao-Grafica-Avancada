
//--------------------------------------------------
//  Autor: Cesar Tadeu Pozzer
//  17/07/2007
//  pozzer3@gmail.com
//--------------------------------------------------

#ifndef ___GRAPH_H__
#define ___GRAPH_H__

//clase para representar um grafo por meio de uma matriz do tipo heightfield 
//onde cada celula possui uma altura. O peso para ir de um no a um de seus 
//8 vizinhos é a diferenca de altura entre os 2 nos + distancia. 

//Instrucoes: dado o seguinte grafo 3x3 (de pesos) com coord min (0,0) e max (2,2):
//  0  0  1   
//  2  0  1   
//  0  1  6   
// --> posso ir do no (0,0) ao no (0,1) com custo (0 + distancia = 0 + 1)
// --> posso ir do no (2,1) ao no (1,2) com custo (0 + distancia = 0 + sqrt(2))
// --> posso ir do no (2,1) ao no (2,2) com custo (5 + distancia = 5 + 1), pois |1-6|

#include <math.h>
#include <stdio.h>
#include <string.h>

#define LARG 10
#define ALT  10

#define WALL 9

class Graph
{
private:
   int m[LARG][ALT]; // matriz que representa os custos do grafo.
   int width;        //dimensoes do grafo
   int height;

public:
   Graph(int c[LARG][ALT])
   { 
      width = LARG;
      height = ALT;
      memcpy(m, c, ALT*LARG*sizeof(int));
   }
	
   //calcula o custo para se locomover do vertice (x1,x2) ao vertice (x2,y2)
   //o custo inclui:
   // - a diferenca de altura entre os vertices
   // - a distancia euclidiana
   float getCost(int x1, int y1, int x2, int y2)
   {
      float cost = (float)fabs( (float)(m[x1][y1] - m[x2][y2]) ) ;
      if( x1 == x2 || y1 == y2 )
         cost += 1.0f;     //distancia em linha reta
      else
         cost += 1.4142f;  //distancia euclidiana na diagonal. Tambem pode ser manhattan. 
      return cost;
   }
    
   //verifica se existe um muro no cenario. 
   bool isWall(int x, int y)
   {
      return (m[x][y] == WALL);     
   }
    
   int getWidth() 
   { 
      return width; 
   }
   int getHeight() 
   { 
      return height; 
   }
    
   //imprime o grafo em formato matricial, com o path selecionado
   void print(List *list)
   {
      printf("\n\n ** Grafo com path **\n");
      for(int c=0; c<=width; c++)
         printf("--");
      printf("\n");
      for(int l=0; l < height; l++)
      {
         for(int c=0; c<width; c++)
         {
            if( c == 0 )    
               printf("|");
            if( list->match(l,c) )
               printf(". ");
            else if( m[l][c] > 0 )
               printf("%d ", m[l][c] );
            else
               printf("  ");

            if( c == width-1 )    
               printf("|");
         }        
         printf(" %d\n",l);
      }     
      for(int c=0; c<=width; c++)
         printf("--");
      printf("\n");
      for(int c=0; c<width; c++)
         printf(" %d", c);
      printf("\n");
   }
};

#endif 
