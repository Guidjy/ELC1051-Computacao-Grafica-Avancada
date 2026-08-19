#include <GL/glut.h>

#include "node.h"
#include "shape.h"
#include "cilindro.h"
#include "group.h"


//faz a geracao das coordenadas que defimen o cilindro em funcao do numero
// de lados e raio .
void cilindro::Gera()
{
   int    face=0;
   double ang, inc;
   double y_1, y_2;

   inc = 2.0*PI/(double)n_lados;

   y_1 = altura/2.0;
   y_2 = -altura/2.0;

   for(ang=0; ang<2*PI; ang+=inc)
   {
      coord_d1[face] = CalculaPosicaoFace(ang, raio_d1, y_1);
      coord_f1[face] = CalculaPosicaoFace(ang, raio_f1, y_1);
      coord_d2[face] = CalculaPosicaoFace(ang, raio_d2, y_2);
      coord_f2[face] = CalculaPosicaoFace(ang, raio_f2, y_2);
      face++;
   }
   gerado=true;  //marca como ja gerado.
}   

//faz a renderizacao do cilindro
void cilindro::Render()
{
   int face=0;
   vetor v0, v1, v2, normal(1,0,0), centro;
   double inc_divisoes;//, y_ini, y_fim;

   if(gerado==false)  //se o cubo nao foi gerado, gera.
      Gera();
   
   MostraDados("Cilindro");

   //o cilindro ainda nao permite textura, apesar de ser facil de ser feito.
   glDisable(GL_TEXTURE_2D);
   
   //------------------------------------------------------------------
   //desenha a face externa
   //------------------------------------------------------------------
   inc_divisoes = altura/(float)resolucao;
   //y_fim = altura/2;
   glBegin(GL_QUAD_STRIP);
   for(face=0; face <= n_lados; face++)
   {
      //calcula a normal de cada face.
      v0.setValor(coord_f1[idx(face)]);
      v2.setValor(coord_f2[idx(face)]);
      v1.setValor(coord_f1[idx(face+1)]);
      //normal = (v1 - v0) ^ (v2 - v1);
      normal = normal.normalize();
      glNormal3f(normal.x, normal.y, normal.z);
      
      glVertex3d(coord_f1[idx(face)].x, coord_f1[idx(face)].y, coord_f1[idx(face)].z);
      glVertex3d(coord_f2[idx(face)].x, coord_f2[idx(face)].y, coord_f2[idx(face)].z);
   }
   glEnd();

   //------------------------------------------------------------------
   //desenha a face de dentro do cilindro
   //------------------------------------------------------------------
   glBegin(GL_QUAD_STRIP);
      for(face=0; face <= n_lados; face++)
      {
         v0.setValor(coord_d1[idx(face)]);
         v1.setValor(coord_d2[idx(face)]);
         v2.setValor(coord_d1[idx(face+1)]);
         //normal = (v1 - v0) ^ (v2 - v1);
         normal = normal.normalize();
         glNormal3f(normal.x, normal.y, normal.z);

         glVertex3d(coord_d1[idx(face)].x, coord_d1[idx(face)].y, coord_d1[idx(face)].z);
         glVertex3d(coord_d2[idx(face)].x, coord_d2[idx(face)].y, coord_d2[idx(face)].z);
      }
   glEnd();

   //------------------------------------------------------------------
   //desenha os lados do cilindro
   //------------------------------------------------------------------
   glBegin(GL_QUAD_STRIP);
      for(face=0; face <= n_lados; face++)
      {
         v0.setValor(coord_f1[idx(face)]);
         v1.setValor(coord_d1[idx(face)]);
         v2.setValor(coord_f1[idx(face+1)]);
         //normal = (v1 - v0) ^ (v2 - v1);
         normal = normal.normalize();
         glNormal3f(normal.x, normal.y, normal.z);

         glVertex3d(coord_f1[idx(face)].x, coord_f1[idx(face)].y, coord_f1[idx(face)].z);
         glVertex3d(coord_d1[idx(face)].x, coord_d1[idx(face)].y, coord_d1[idx(face)].z);
      }
   glEnd();

   //parte de baixo do cilindro
   glBegin(GL_QUAD_STRIP);
      for(face=0; face <= n_lados; face++)
      {
         v0.setValor(coord_f2[idx(face)]);
         v2.setValor(coord_d2[idx(face)]);
         v1.setValor(coord_f2[idx(face+1)]);
         //normal = (v1 - v0) ^ (v2 - v1);
         normal = normal.normalize();
         glNormal3f(normal.x, normal.y, normal.z);

         glVertex3d(coord_f2[idx(face)].x, coord_f2[idx(face)].y, coord_f2[idx(face)].z);
         glVertex3d(coord_d2[idx(face)].x, coord_d2[idx(face)].y, coord_d2[idx(face)].z);
      }
   glEnd();
}
