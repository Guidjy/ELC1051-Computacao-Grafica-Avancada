//-------------------------------------------------------------------------
// classe para definicao da fonte de luz e classes derivadas
//   - point light source
//   - spot light source
//
// by Cesar Pozzer 29/04/2001
//-------------------------------------------------------------------------

class lightsource: public node{
public:
   GLfloat pos[4], amb[4], dif[4], esp[4], shi[1];
   float raio;
   int SHADE_MODEL;
   int ligada;

   lightsource(char *name): node(name)
   {
       pos[0] = 0.0f;  pos[1] = 0.0f,  pos[2] = 0.0f;   pos[3] = 1.0f;
      amb[0] = 1.5f;  amb[1] = 0.5f,   amb[2] = 0.5f;   amb[3] = 1.0f;
      dif[0] = 0.4f;  dif[1] = 0.4f,   dif[2] = 0.4f;   dif[3] = 1.0f;
      esp[0] = 0.3f;  esp[1] = 0.3f,   esp[2] = 0.3f;   esp[3] = 1.0f;
      shi[0] = 10;
      raio = 0;
      ligada = 1;
   }

   lightsource()
   {
       pos[0] = 0.0f;  pos[1] = 0.0f,  pos[2] = 0.0f;   pos[3] = 1.0f;
      amb[0] = 0.5f;  amb[1] = 0.5f,   amb[2] = 0.5f;   amb[3] = 1.0f;
      dif[0] = 0.4f;  dif[1] = 0.4f,   dif[2] = 0.4f;   dif[3] = 1.0f;
      esp[0] = 0.3f;  esp[1] = 0.7f,   esp[2] = 0.3f;   esp[3] = 1.0f;
      shi[0] = 4;
      raio = 0;
      ligada = 1;
   }
   
   //especifica como sera a renderizacao da esfera que representa a fonte
   void setShadeModel(int model)
   {
      //GL_FLAT
      //GL_SMOOOTH
      SHADE_MODEL = model;
   }

   //liga ou desliga a luz, dependendo do estado anterior
   void setLuzLigada()
   {
      if(ligada==1)
      {
         ligada=0;
      }
      else
      {
         ligada=1;
      }
   }

   void SetPosition(float x, float y, float z)
   {
      pos[0] = x;  pos[1] = y;  pos[2] = z;   pos[3] = 1.0f;
   }

   void SetAmbient(float r, float g, float b)
   {
      amb[0] = r;   amb[1] = g;   amb[2] = b;   amb[3] = 1.0f;
   }

   void SetDiffuse(float r, float g, float b)
   {
      dif[0] = r;   dif[1] = g;   dif[2] = b;   dif[3] = 1.0f;
   }

   void SetSpecular(float r, float g, float b)
   {
      esp[0] = r;   esp[1] = g,   esp[2] = b;   esp[3] = 1.0f;
   }
   
   void SetShiness(float shiness)
   {
      shi[0] = shiness;
   }

   //determina o raio da esfera que define a fonte
   void SetRaio(float r)
   {
      raio = r;
   }
      
   virtual void Render()
   {
   }
};


class plight: public lightsource{
public:
   plight(char *name): lightsource(name)
   {
       pos[0] = 0.0f;  pos[1] = 0.0f,  pos[2] = 0.0f;   pos[3] = 1.0f;
   }
   
   plight()
   {
       pos[0] = 0.0f;  pos[1] = 0.0f,  pos[2] = 0.0f;   pos[3] = 1.0f;
   }

   void Render()
   {
      MostraDados("eu sou uma fonte pontual");

	  glPushMatrix();
         GLfloat color[ ] = {1,1,0.2F,1};
         glMaterialfv(GL_FRONT,GL_EMISSION,color); 
         glTranslated(pos[0], pos[1], pos[2]);
         glutSolidSphere(raio,14,16);
	  glPopMatrix();
        
      GLfloat none[ ] = {0,0,0,1};
      glMaterialfv(GL_FRONT,GL_EMISSION,none);
   }
   
   //habilita a luz, dependendo da variavel ligada.
   int SetupLights()
   {
      if(ligada==1)
         glEnable (GL_LIGHT0);
      else
         glDisable (GL_LIGHT0);

      glLightfv(GL_LIGHT0, GL_POSITION, pos);
      glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);      
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
      glLightfv(GL_LIGHT0, GL_SPECULAR, esp);
      glLightfv(GL_LIGHT0, GL_SHININESS, shi);

      return 1;
   }
};


class spotlight: public lightsource{
public:
   int cutoff;
   GLfloat direcao[3];  //direcao da luz spot
   spotlight(char *name): lightsource(name)
   {
       cutoff = 10;
       direcao[0] = 0;
       direcao[1] = -1;
       direcao[2] = 0;
   }

   spotlight()
   {
       cutoff = 10;
       direcao[0] = 0;
       direcao[1] = -1;
       direcao[2] = 1;
   }

   //especifica os dados da fonte direcional.
   void SetApperture(int apperture)
   {
      cutoff = apperture;
   }

   //especifica os dados da fonte direcional.
   void setOrientacao(float x,float y, float z)
   {
      direcao[0] = x;
      direcao[1] = y;
      direcao[2] = z;
   }
   
   void Render()
   {
      MostraDados("eu sou uma fonte spot");
  	  glPushMatrix();
         GLfloat color[ ] = {1,1,0.2F,1};
         glMaterialfv(GL_FRONT,GL_EMISSION,color); /* it is a source of light */
         glTranslated(pos[0]-raio/2, pos[1]-raio/2, pos[2]-raio/2);
         glutSolidSphere(raio,14,16);
	  glPopMatrix();

      /* restore material */
      GLfloat none[ ] = {0,0,0,1};
      glMaterialfv(GL_FRONT,GL_EMISSION,none);
   }

   int SetupLights()
   {
      if(ligada==1)
         glEnable (GL_LIGHT1);
      else
         glDisable (GL_LIGHT1);

      glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direcao);
      glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
      glLightfv(GL_LIGHT1, GL_POSITION, pos);
      //glLightfv(GL_LIGHT1, GL_AMBIENT,  amb);      
      glLightfv(GL_LIGHT1, GL_DIFFUSE,  dif);
      //glLightfv(GL_LIGHT1, GL_SPECULAR, esp);
      //glLightfv(GL_LIGHT1, GL_SHININESS, shi);

      return 1;
   }
};
