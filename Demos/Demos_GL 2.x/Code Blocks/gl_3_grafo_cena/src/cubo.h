//-------------------------------------------------------------------------
// classe que contem a definicao de paralelepipeldos. Alem do tamanho,
// pode-se especificar:
//   - resolucao -> numero de divisoes de cada face (no caso de iluminacao)
//
// o cubo e' gerado centrado na origem, com a coordenada y (altura) para cima da tela
// e z (profundidade) para o fundo.
// by Cesar Pozzer 04/05/2001


// a versao 4 tem a textura para 1 poligono
//-------------------------------------------------------------------------



class cubo: public shape{
   float altura,     largura,     profund;     //dimensao do objeto
   float res_altura, res_largura, res_profund; //resolucao do objeto
   float max_ext;        //maxima extensao (maior lado) do paralelepipedo
   float escala_textura; //indica como sera a replicacao da textura. Quantom
                    //maior for, maior sera a quantidade de texturas aplicadas
   long  textura;        //indica qual face recebera textura.
   vetor p[9];
   bool  invertNormal;
public:
   cubo(char *name): shape(name)
   {
      res_altura = res_largura = res_profund = 1; //resolucao do objeto
      altura     = largura     = profund = 1.0; //dimensoes do objeto
      textura = 0x111111;
      invertNormal = false;
      escala_textura  = 1;
   }

   cubo()
   {
      res_altura = res_largura = res_profund = 1; //resolucao do objeto
      altura     = largura     = profund = 1.0; //dimensoes do objeto
      invertNormal = false;
      textura = 0x111111;
      escala_textura = 1;
   }

   void setNormal(bool b)
   {
      invertNormal = b;
   }

   //determina as dimensoes do cubo, centrado na origem
   void setDimensoes(float larg, float alt, float prof)
   {
      altura = alt;  
      largura = larg; 
      profund = prof;
      max_ext = (altura>largura)? altura : largura;
      max_ext = (profund>max_ext)? profund : max_ext;
   }

   //determina como cada face sera subdividida para melhorar a iluminacao
   void setResolucao(int larg, int alt, int prof, float textura)
   {
      res_largura = larg; 
      res_altura  = alt;  
      res_profund = prof;
      escala_textura = textura;
   }

   void setTextura(long tex)
   {
      textura = tex;
   }

   void Gera()
   {
      p[1].setValor(-largura/2, altura/2,  profund/2);  
      p[2].setValor(largura/2,  altura/2,  profund/2);  
      p[3].setValor(largura/2,  altura/2,  -profund/2);   
      p[4].setValor(-largura/2, altura/2,  -profund/2);   
      p[5].setValor(-largura/2, -altura/2, profund/2);  
      p[6].setValor(largura/2,  -altura/2, profund/2);  
      p[7].setValor(largura/2,  -altura/2, -profund/2);   
      p[8].setValor(-largura/2, -altura/2, -profund/2);   
      gerado=true;  //marca como ja gerado.
   }   

   //metodo definido em cubo.cpp
   void Render();
};
