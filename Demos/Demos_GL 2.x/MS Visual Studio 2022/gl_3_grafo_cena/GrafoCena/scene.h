//-------------------------------------------------------------------------
// classe principal que contem um grupo. Nesta classe sao chamados os metodos
// para fazer o posicionamento da camera e fontes luminosas.
// E' chamado o metodo render() de group que mostra a cena na tela, segundo 
// infomracoes de camera e luzes.
//
// by Cesar Pozzer 29/04/2001
//-------------------------------------------------------------------------

class scene: public group{
public:
   scene()
   {
   }

   void Render()
   {
      if (SetupLights())
      {
         //printf(" \n Encontrou fonte luminosa");
         glEnable(GL_LIGHTING);
      }
      else
      {
         ;//printf(" \n Nao encontrou fonte luminosa");
      }
      if(SetupCamera())
         ;//printf(" \n Encontrou Camera");
      else
         ;//printf(" \n Nao Encontrou Camera");

      group::Render();
   }
};
