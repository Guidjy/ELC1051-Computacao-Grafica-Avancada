#include <stdio.h>
#include <time.h>
#include "Vetor.h"
#include <string.h>


extern int posicao;

class node{
   //int  mask;     //se e' visivel, selecionavel, ...
public:
   int id;
   char nome[80], tmp[80]; //nome do no'

   node()
   {
      strcpy(nome,"Sem nome");
      id = 0;
   }
   node(char *name)
   {
      id = 0;
      if(strlen(name)>79)
      {
         printf("\nTRUNCOU\n");
         strncpy(tmp, name, 78);
         tmp[39] = '\0';
         strcpy(nome,tmp);
      }
      else
         strcpy(nome,name);
   }

   //caso o no' nao for camera, nao tem o metodo SetupCamera e assim retorna 0
   virtual int SetupCamera()
   {
      return 0;
   }

   //caso o no' nao for light, nao tem o metodo setupLight e por isso deve retornar 0
   virtual int SetupLights()
   {
      return 0; 
   }     

   //Esta funcao chama todos os metodos virtuais dos objetos que pertencem
   //a lista de itens do group. Pode ser transformacao, objeto, fonte, camera, etc...
   //Caso o no nao tiver o metodo Render, nada e' feito
   virtual void Render()
   {

   }

   //imprime o nome associado ao no'. Este metodo e' chamdo pelo Render()
   void MostraDados(char *info)
   {
      /*int i;
      
      printf("\n");
      for(i=0; i<posicao; i++)
         printf(" ");
      printf("%s --> \"%s\"" ,info,  nome);   
                */
   }
};
