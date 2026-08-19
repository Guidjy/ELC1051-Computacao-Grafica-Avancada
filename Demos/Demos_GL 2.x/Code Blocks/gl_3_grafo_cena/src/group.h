//-------------------------------------------------------------------------
// classe que contem um conjunto de nos dos tipos
//   - transformacao; (rotacao, escala, translacao)
//   - shape;         (primitiva geometrica - cubo, esfera, engrenagem, cilindro, ...)
//   - camera;        (ortho, perspect)
//   - light source.  (point, spot, direcional)
//   Uma transformacao tambem pode ter um group como filho, e assim recursivamente.
//  Os nos filhos da transformacao sao processados com os valores associados a
//  transformacao. A tranformacao e' vista como um no do grupo, que pertence a cena
//  ou a outra transformacao.
//
// by Cesar Pozzer 29/04/2001
//-------------------------------------------------------------------------


class group: public node{
   node *child[100]; //nos filhos
   int num_children;
public:
   group(char *name):node(name)
   {
      num_children = 0;
   }
   group()
   {
      num_children = 0;
   }

   //faz traversal ate encontrar uma camera. Se nao encontrar nenhuma, retorna 0.
   int SetupCamera()
   {
      int i;
      for (i=0; i<num_children; i++)
      {
         if(child[i]->SetupCamera())
            return 1;
      }
      return 0;
   }

   //faz traversal para posicionar e habilitar as fontes luminosas
   int SetupLights()
   {
      int n=0, i;
      for (i=0; i<num_children; i++)
      {
         n += child[i]->SetupLights();
      }
      return n;
   }

   //adiciona um novo no ao grupo. Se o no for uma transformacao, este pode ter
   //outros grupos associados.
   void add(node *n)
   {
      if(num_children<100)
         child[num_children++] = n;
      else
         printf("\n O numero de filhos nao pode ser maior que 100");
   }

   void Render()
   {
      int i;
      for (i=0; i<num_children; i++)
      {
         child[i]->Render();
         //printf(" %d",i);
      }
   }
};
