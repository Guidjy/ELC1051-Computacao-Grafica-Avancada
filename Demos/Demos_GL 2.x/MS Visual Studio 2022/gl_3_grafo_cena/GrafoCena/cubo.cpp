#include <GL/glut.h>

#include "node.h"
#include "shape.h"
#include "cubo.h"
#include "group.h"
#include "Vetor.h"

void cubo::Render()
{
   int face=0;
   vetor  v0, v1, normal, centro;
   double inc_largura, inc_altura, inc_profund;
   double x_ini, x_fim, y_ini, y_fim, z_ini, z_fim;
   
   float x_tex, y_tex;
   float larg_tex_x, larg_tex_y;
   float inc_tex_x, inc_tex_y;

   if(gerado==false)  //se o cubo nao foi gerado, gera.
      Gera();
   
   MostraDados("Cubo");

   //----------------------------------------------------
   //calcula os incrementos e posicao final de cada face
   //----------------------------------------------------
   x_fim = largura/2;
   y_fim = altura/2;
   z_fim = profund/2;
   inc_largura = largura/(float)res_largura;
   inc_altura  = altura/(float)res_altura;
   inc_profund = profund/(float)res_profund;



   //------------------------------------------------------------------
   //******************************************************************
   //desenha a tampa superior
   //******************************************************************
   //------------------------------------------------------------------
   //calcula a normal da face.
   v0 = p[4] - p[1];
   v1 = p[3] - p[4];
   normal = v1 ^ v0;
   normal = normal.normalize();
   if( invertNormal == true )
      normal.inverte();
   glNormal3f(normal.x, normal.y, normal.z);

   //verifica se a textura para a face de cima esta habilitada
   if (textura&0x100000)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);
   
   //calcula as dimensoes da textura para nao haver distorcao em S e T.
   larg_tex_x = largura/max_ext;
   larg_tex_y = profund/max_ext;

   inc_tex_x = (larg_tex_x/res_largura)*escala_textura;
   inc_tex_y = (larg_tex_y/res_profund)*escala_textura;

#define STRIP

#ifdef STRIP
   //faz a subdivisisao da face em malha, segundo o valores de resolucao.
   y_ini = altura/2;
   y_tex = 0;
   for(z_ini = -profund/2; z_ini <= z_fim-0.001; z_ini+=inc_profund)
   {
      x_tex = 0.0;
      glBegin(GL_QUAD_STRIP);
              
      for(x_ini = -largura/2; x_ini <= x_fim+0.001; x_ini+=inc_largura)
      {   

         glTexCoord2f(x_tex, y_tex);         glVertex3d(x_ini,  y_ini, z_ini);
         glTexCoord2f(x_tex, y_tex+inc_tex_y); glVertex3d(x_ini,  y_ini, z_ini+inc_profund);
         x_tex += inc_tex_x;
      }
      glEnd();
      y_tex+=inc_tex_y;
    }
#else
    glBegin(GL_TRIANGLES);
       glVertex3f(largura/2, altura/2, -profund/2);
       glVertex3f(-largura/2, altura/2, -profund/2);
       glVertex3f(-largura/2, altura/2, profund/2);

       glVertex3f(largura/2, altura/2, profund/2);
       glVertex3f(largura/2, altura/2, -profund/2);
       glVertex3f(-largura/2, altura/2, profund/2);
    glEnd();
#endif

   //------------------------------------------------------------------
   //*****************************************************************
   //desenha a tampa inferior
   //*****************************************************************
   //------------------------------------------------------------------
   v0 = p[5] - p[6]; //OK
   v1 = p[5] - p[8];
   normal = v1 ^ v0;
   normal = normal.normalize();
   if( invertNormal == true )
      normal.inverte();
   glNormal3f(normal.x, normal.y, normal.z);

   if (textura&0x010000)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);
   
   larg_tex_x = largura/max_ext;
   larg_tex_y = profund/max_ext;

   inc_tex_x = (larg_tex_x/res_largura)*escala_textura;
   inc_tex_y = (larg_tex_y/res_profund)*escala_textura;

   y_ini = -altura/2;
   x_tex = 0.0;
   for(x_ini = -largura/2; x_ini <= x_fim-0.001; x_ini+=inc_largura)
   {
      y_tex = 0;
      glBegin(GL_QUAD_STRIP);
      for(z_ini = -profund/2; z_ini <= z_fim+0.001; z_ini+=inc_profund)
      {   
         glTexCoord2f(x_tex,           y_tex); glVertex3d(x_ini,             y_ini, z_ini);
         glTexCoord2f(x_tex+inc_tex_x, y_tex); glVertex3d(x_ini+inc_largura, y_ini, z_ini);
         y_tex += inc_tex_y;
      }
      glEnd();
      x_tex+=inc_tex_x;
   }

   
   //------------------------------------------------------------------
   //*****************************************************************
   //desenha a face da frente
   //*****************************************************************
   //------------------------------------------------------------------
   v0 =p[1] - p[2];
   v1 =p[1] - p[5];
   normal = v0 ^ v1;
   normal = normal.normalize();
   if( invertNormal == true )
      normal.inverte();
   glNormal3f(normal.x, normal.y, normal.z);

   if (textura&0x001000)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);

   z_fim = -profund/2;
   x_fim = largura/2;
   y_fim = altura/2;
   y_tex = 0.0;
   x_tex = 0.0;
   
   larg_tex_x = largura/max_ext;
   larg_tex_y = altura/max_ext;

   inc_tex_x = (larg_tex_x/res_largura)*escala_textura;
   inc_tex_y = (larg_tex_y/res_altura)*escala_textura;

   //desenha cada fileira em z
   y_tex = 0.0;
   for(y_ini = -altura/2 ;y_ini <= y_fim-0.001; y_ini+=inc_altura)
   {
      x_tex = 0.0;
      glBegin(GL_QUAD_STRIP);
      for(x_ini = -largura/2; x_ini <= x_fim+0.001; x_ini+=inc_largura)
      {   
         glTexCoord2f(x_tex, y_tex);           glVertex3d(x_ini, y_ini,            z_fim);
         glTexCoord2f(x_tex, y_tex+inc_tex_y); glVertex3d(x_ini, y_ini+inc_altura, z_fim);
         x_tex += inc_tex_x;
      }
      glEnd();
      y_tex += inc_tex_y;
   }

   //------------------------------------------------------------------
   //*****************************************************************
   //desenha a face de traz
   //*****************************************************************
   //------------------------------------------------------------------
   v0 = p[4] - p[3];
   v1 = p[4] - p[7];
   normal = v1 ^ v0;
   normal = normal.normalize();
   if( invertNormal == true )
      normal.inverte();
   glNormal3f(normal.x, normal.y, normal.z);

   if (textura&0x000100)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);

   z_fim = profund/2;
   x_fim = largura/2;
   y_fim = altura/2;
   y_tex = 0.0;
   x_tex = 0.0;
   
   larg_tex_x = largura/max_ext;
   larg_tex_y = altura/max_ext;

   inc_tex_x = (larg_tex_x/res_largura)*escala_textura;
   inc_tex_y = (larg_tex_y/res_altura)*escala_textura;

   x_tex = 0.0;
   //desenha cada fileira em z
   for(x_ini = -largura/2; x_ini <= x_fim-0.001; x_ini+=inc_largura)
   {
      y_tex = 0.0;
      glBegin(GL_QUAD_STRIP);
      for(y_ini = -altura/2 ;y_ini <= y_fim+0.001; y_ini+=inc_altura)
      {   
         glTexCoord2f(x_tex, y_tex);           glVertex3d(x_ini,             y_ini, z_fim);
         glTexCoord2f(x_tex+inc_tex_x, y_tex); glVertex3d(x_ini+inc_largura, y_ini, z_fim);
         y_tex += inc_tex_y;
      }
      glEnd();
      x_tex += inc_tex_x;
   }



   //------------------------------------------------------------------
   //*****************************************************************
   //desenha a face da esquerda
   //*****************************************************************
   //------------------------------------------------------------------
   v0 = p[1] - p[4];
   v1 = p[1] - p[5];
   normal = v0 ^ v1;
   normal = normal.normalize();
   if( invertNormal == true )
      normal.inverte();
   glNormal3f(normal.x, normal.y, normal.z);

   if (textura&0x000010)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);

   x_ini = -largura/2;
   z_fim = profund/2;
   x_fim = largura/2;
   y_fim = altura/2;
   y_tex = 0.0;
   
   larg_tex_x = profund/max_ext;
   larg_tex_y = altura/max_ext;

   inc_tex_x = (larg_tex_x/res_profund)*escala_textura;
   inc_tex_y = (larg_tex_y/res_altura)*escala_textura;

   //desenha cada fileira em z
   x_tex = 0.0;
   for(z_ini = -profund/2 ;z_ini <= z_fim-0.001; z_ini+=inc_profund)
   {
      y_tex = 0.0;
      glBegin(GL_QUAD_STRIP);
      for(y_ini = -altura/2 ;y_ini <= y_fim+0.001; y_ini+=inc_altura)
      {   
         glTexCoord2f(x_tex, y_tex          ); glVertex3d(x_ini, y_ini, z_ini);
         glTexCoord2f(x_tex+inc_tex_x, y_tex); glVertex3d(x_ini, y_ini, z_ini+inc_profund);
         y_tex += inc_tex_y;
      }
      glEnd();
      x_tex += inc_tex_x;
   }

   
   //------------------------------------------------------------------
   //*****************************************************************
   //desenha a face da direita
   //*****************************************************************
   //------------------------------------------------------------------
   v0 = p[2] - p[3];
   v1 = p[2] - p[6];
   normal = v1 ^ v0;
   normal = normal.normalize();
   if( invertNormal == true )
      normal.inverte();
   glNormal3f(normal.x, normal.y, normal.z);

   if (textura&0x000001)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);

   x_ini = largura/2;
   z_fim = profund/2;
   x_fim = largura/2;
   y_fim = altura/2;
   y_tex = 0.0;
   
   larg_tex_x = profund/max_ext;
   larg_tex_y = altura/max_ext;

   inc_tex_x = (larg_tex_x/res_profund)*escala_textura;
   inc_tex_y = (larg_tex_y/res_altura)*escala_textura;

   //desenha cada fileira em z
   y_tex = 0.0;
   for(y_ini = -altura/2 ;y_ini <= y_fim-0.001; y_ini+=inc_altura)
   {
      x_tex = 0.0;
      glBegin(GL_QUAD_STRIP);
      for(z_ini = -profund/2 ;z_ini <= z_fim+0.001; z_ini+=inc_profund)
      {   
         glTexCoord2f(x_tex, y_tex          ); glVertex3d(x_ini, y_ini,            z_ini);
         glTexCoord2f(x_tex, y_tex+inc_tex_y); glVertex3d(x_ini, y_ini+inc_altura, z_ini);
         x_tex += inc_tex_x;
      }
      glEnd();
      y_tex += inc_tex_y;
   }

   //glPopMatrix( );
   /**/
}
