//-------------------------------------------------------------------------
// classe para definicao de aparencias classes derivadas
//   - material
//   - fog
//   - texture
//
// by Cesar Pozzer 29/04/2001
//-------------------------------------------------------------------------

#include "Bmp.h"

class appearance: public node{
public:
   appearance(char *name) : node(name)
   {
   }
   appearance()
   {
   }
   
   virtual void Render()
   {
   }
};


class material: public appearance{
     float _mat_amb[4], _mat_dif[4], _mat_esp[4], _mat_shi[1];
public:
    material(char *name): appearance(name)
   {
      _mat_amb[0] = 0.5f;   _mat_amb[1] = 0.5f;   _mat_amb[2] = 0.5f;   _mat_amb[3] = 0.8f;
      _mat_dif[0] = 0.6f;   _mat_dif[1] = 0.6f;   _mat_dif[2] = 0.6f;   _mat_dif[3] = 0.2f;
      _mat_esp[0] = 0.4f;   _mat_esp[1] = 0.4f;   _mat_esp[2] = 0.4f;   _mat_esp[3] = 0.2f;
      _mat_shi[0] = 25.0f;
   }

    material()
   {
      _mat_amb[0] = 0.5f;   _mat_amb[1] = 0.5f;   _mat_amb[2] = 0.5f;   _mat_amb[3] = 0.8f;
      _mat_dif[0] = 0.6f;   _mat_dif[1] = 0.6f;   _mat_dif[2] = 0.6f;   _mat_dif[3] = 0.2f;
      _mat_esp[0] = 0.4f;   _mat_esp[1] = 0.4f;   _mat_esp[2] = 0.4f;   _mat_esp[3] = 0.2f;
      _mat_shi[0] = 25.0f;
   }

   void SetAmbient(float r, float g, float b, float alfa)
   {
      _mat_amb[0] = r; _mat_amb[1] = g; _mat_amb[2] = b; _mat_amb[3] = alfa;
   }

   void SetDiffuse(float r, float g, float b, float alfa)
   {
      _mat_dif[0] = r; _mat_dif[1] = g; _mat_dif[2] = b;  _mat_dif[3] = alfa;
   }
   
   void SetSpecular(float r, float g, float b, float alfa)
   {
      _mat_esp[0] = r; _mat_esp[1] = g; _mat_esp[2] = b;  _mat_esp[3] = alfa;
   }

   void SetShiness(float shi)
   {
      _mat_shi[0] = shi;
   }

   //renderiza o material. Desabilita a textura e habilita a luz
   void Render()
   {
//printf("material");
      glEnable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      MostraDados("Material");
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, _mat_amb);
      glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, _mat_dif);
      glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, _mat_esp);
      glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, _mat_shi);
   }
};

//a textura e' armazenada em um vetor, que possui valores R, G e B. Nao e' usada
//matriz bidimensional porque da erro no mapeamento se o tamanho default desta
//matriz for maior que o tamanho da imagem de textura, na funcao BindTexture().  
class texture: public appearance{
   Bmp *img;
   GLuint textureID;
public:   
   texture(char *name): appearance(name)
   {
      textureID = 0;
   }
   texture()
   {
      textureID = 0;
   }

   void bindTexture(Bmp *img)
   {
      this->img = img;
      initTexture();
   }


   //faz a configuracao da textura
   void initTexture(void)
   {
      glDepthFunc(GL_LEQUAL);

      glGenTextures( 1, &textureID );
      glBindTexture( GL_TEXTURE_2D, textureID );

      glTexImage2D(GL_TEXTURE_2D, 
                   0, 
                   GL_RGB, 
                   img->getWidth(), 
                   img->getHeight(), 
                   0, 
                   GL_RGB, 
                   GL_UNSIGNED_BYTE, 
                   img->getImage() );

      
      glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

      //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      
      //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_SMOOTH);
      //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_SMOOTH);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
      
      glEnable(GL_TEXTURE_2D);
      //glShadeModel(GL_FLAT);
   }
   
   void Render()
   {
      glEnable( GL_TEXTURE );
      glEnable( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, textureID );
      MostraDados("Textura");
   }
};

//classe nao implementada na aplicacao
class fog: public appearance{
public:   
   fog(char *name): appearance(name)
   {
   }
   fog()
   {
   }
   
   void Render()
   {
      MostraDados("Fog");
   }
};


class color: public appearance{
public:   
   float cor[4];
   
   color(char *name): appearance(name)
   {
      cor[0] = 1; cor[1] = 0; cor[2] = 0; cor[3] = 0; 
   }
   color()
   {
      cor[0] = 1; cor[1] = 0; cor[2] = 0; cor[3] = 0; 
   }
   
   void setColor(float r, float g, float b, float alfa)
   {
      cor[0] = r; cor[1] = g; cor[2] = b; cor[3] = alfa;
   }

   void Render()
   {
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_LIGHTING);
      glColor3fv(cor);
      MostraDados("Cor");
   }
};
