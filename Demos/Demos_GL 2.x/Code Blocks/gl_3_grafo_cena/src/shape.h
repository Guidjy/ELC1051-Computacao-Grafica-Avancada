//-------------------------------------------------------------------------
// classe para definicao de objetos geometricos
//   - cubo
//   - cilindro
//   - esfera
//   - engrenagem
//
// by Cesar Pozzer 29/04/2001
//-------------------------------------------------------------------------

class shape: public node{
public:
   float eixo_x, eixo_y, eixo_z;  //eixo de rotacao proprio do objeto
   bool  gerado;

   shape(char *name) : node (name)
   {
      gerado = false;
   }
   
   shape()
   {
      gerado = false;
   }
};
