//////////////////////////////////////////////////////////
// classe para definicao de transformacoes. Permite rotacao e translacao.
//

//////////////////////////////////////////////////////////

extern int posicao;

class transform: public group{
   double tx, ty, tz;
   double ang, eixo_x, eixo_y, eixo_z;
   double anim_ang, anim_eixo_x, anim_eixo_y, anim_eixo_z;
   float velocidade_rotacao;    //velocidade de rotacao propria do objeto
   float angulo_rotacao_atual;  //posicao de rotacao
public:
   transform(char *name): group(name)
   {
      tx = ty = tz =0.0;
      ang = eixo_x = eixo_y = 0.0;
      eixo_z = 1.0;
       velocidade_rotacao   = 1.0;
      angulo_rotacao_atual = 0.0; 
   }
   transform()
   {
      tx = ty = tz =0.0;
      ang = eixo_x = eixo_y = 0.0;
      eixo_z = 1.0;
       velocidade_rotacao   = 1.0;
      angulo_rotacao_atual = 0.0; 
   }
   
   void Render()
   {
      MostraDados("Transformacao");
      posicao+=5;
      
      glPushMatrix();
      glTranslated(tx, ty, tz);
      glRotated(ang, eixo_x, eixo_y, eixo_z);

      if( anim_ang > 0.01 )
      {
         angulo_rotacao_atual += anim_ang;
         glRotated(angulo_rotacao_atual, anim_eixo_x, anim_eixo_y, anim_eixo_z);
      }

      //glRotated(ang, 0,1,0);
      group::Render();
      glPopMatrix();

      posicao-=5;
   }

   int SetupCamera()
   {
      if(group::SetupCamera())
      {
         printf("Posicionou camera pela transformacao" );
         return 1;
      }
      return 0;
   }

   int SetupLights()
   {
      int lights = 0;
      glPushMatrix();
      glTranslated(tx, ty, tz);
      glRotated(ang, eixo_x, eixo_y, eixo_z);

      if( anim_ang > 0.01 )
      {
         angulo_rotacao_atual += anim_ang;
         glRotated(angulo_rotacao_atual, anim_eixo_x, anim_eixo_y, anim_eixo_z);
      }

      lights =  group::SetupLights();
      glPopMatrix();
      return lights;
   }

   void setRotacao(double angulo, double rotx, double roty, double rotz)
   {
      ang = angulo;
      eixo_x  = rotx;
      eixo_y  = roty;
      eixo_z  = rotz;
   }

   void setTranslacao(double tranx, double trany, double tranz)
   {
      tx  = tranx;
      ty  = trany;
      tz  = tranz;
   }

   void setAnimacao(double angulo, double rotx, double roty, double rotz)
   {
      anim_ang = angulo;
      anim_eixo_x = rotx;
      anim_eixo_y = roty;
      anim_eixo_z = rotz;      
   }
};
