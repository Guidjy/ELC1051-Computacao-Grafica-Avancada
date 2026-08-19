//-------------------------------------------------------------------------
// classe que contem a definicao de esferas 
// by Cesar Pozzer 07/05/2001
//-------------------------------------------------------------------------

class esfera: public shape{
   double res_x, res_y;
   double raio;
public:

   esfera()
   {
      res_x = res_y = 5;
      raio = 3;
   }

   esfera(char *name): shape(name)
   {
      res_x = res_y = 5;
      raio = 3;
   }

   void setResolucao(int x, int y)
   {
      if(x<5 || x>99)
         res_x = 5;
      else
         res_x = x;
      if(y<5 || y>99)
         res_y = 5;
      else
         res_y = y;
   }

   void setRaio(double r)
   {
      raio = r;
   }

   void Render()
   {
      static GLUquadricObj* qobj = NULL;
      if (!qobj)
        qobj = gluNewQuadric();
      
      MostraDados("Esfera");

      //seta o tipo de renderizacao: FLAT OU SMOOTH
      //glShadeModel(SHADE_MODEL);

      gluSphere(qobj,raio,(int)res_x, (int)res_y);
   }
};
