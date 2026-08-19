//-------------------------------------------------------------------------
// classe que contem a definicao de cilindros e cones. Alem do tamanho,
// pode-se especificar:
//
// o cilindro e' gerado centrado na origem, com a coordenada y (altura) para cima da tela
// e z (profundidade) para o fundo.
// by Cesar Pozzer 07/05/2001
//-------------------------------------------------------------------------

#define PI   3.1415926535897932

class cilindro: public shape{
   int    tampa; //se e' tampado=1 ou oco=2
   int    n_lados, resolucao;
   double raio_d1, raio_f1; //raio de dentro e fora do lado 1
   double raio_d2, raio_f2; //raio de dentro e fora do lado 2
   vetor  coord_d1[100]; //coordenada do eixo da engrenagem
   vetor  coord_d2[100];
   vetor  coord_f1[100]; //coordenada dos dentes da engrenagem
    vetor  coord_f2[100];
   double altura;
public:

   cilindro()
   {
      n_lados   = 6;
      resolucao = 2;
      raio_d1   = raio_d2 = 1;
      raio_f1   = raio_f2 = 2;
   }

   cilindro(char *name): shape(name)
   {
      n_lados   = 6;
      resolucao = 2;
      raio_d1   = raio_d2 = 1;
      raio_f1   = raio_f2 = 2;
   }

   void setResolucao(int lados, int resoluc)
   {
      if(lados<3 || lados>99)
         n_lados = 6;
      else
         n_lados = lados;
      if(resoluc<1)
         resolucao = 2;
      else
         resolucao = resoluc;
   }

   //determina os raios do lado direito do cilindro
   void setRaios_1(double dentro, double fora)
   {
      raio_d1 = dentro;
      raio_f1 = fora;
   }

   //determina os raios do lado esquerdo do cilindro
   void setRaios_2(double dentro, double fora)
   {
      raio_d2 = dentro;
      raio_f2 = fora;
   }

   //determina a altura em y do cilindro
   void setAltura(double larg)
   {
      altura = larg;
   }

   //funcao que simula uma lista circular. Usada para calcular o vetor normal de
   //cada face do cilindro.
   int idx(int face)
   {
      if(face>=n_lados)
         return (face-n_lados);
      else
         return face;
   }

   //usado funcoes trigonometricas, esta funcao determina a posicao de cada vertice
   //que gera o cilindro. 
   vetor CalculaPosicaoFace(double ang, double raio, double y)
   {
      vetor coord;
      coord.x = cos(ang)*raio;
      coord.y = y;
      coord.z = sin(ang)*raio;
      return coord;
   }

   //metodos definidos em cilindro.cpp
   void Gera();
   
   void Render();
};
