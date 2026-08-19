#ifndef __LINE___H__
#define __LINE___H__

#include <GL/glut.h>

#include "Vector.h"
#include "Circle.h"

class Line
{
public:
   Vector p, d; //position (p) and direction (d). Direction is a vector
                //with lenght equals line lenght in respect to the origin.

   Line(Vector _p, Vector _d)
   {
      p = _p;
      d = _d;
   }

   //calculo de intesercao entre linhas
   //s e t representam as coordenadas parametricas do ponto de intersecao.
   bool intersects(Line &l, float &s, float &t)
   {
      //determinant - diz se o sistema eh LI ou LD
      float Det = l.d.x*d.y - l.d.y*d.x;
      Vector Diff( l.p - p );

      //se determinante for zero, daria erro de vivisao por zero.
      //Ocorre quanto direcoes sao coincidentes, ou seja, linhas parelelas.
      //Neste caso s ou t seriam infinito
      if (Det*Det > 0.001 * d.norm() * l.d.norm() )
      {
         double InvDet = 1.0/Det;
         s = (l.d.x*Diff.y - l.d.y*Diff.x)*InvDet;
         t = (  d.x*Diff.y -   d.y*Diff.x)*InvDet;

         if( t > 0 && t < 1 && s > 0 && s < 1 )
            return true;
      }
      else //linhas paralelas
      {
         return false;
      }
      return false;
   }


   /**
    * Verifica se uma circunferencia centrada em c.p com diametro definido por c.r é interceptada por
    * uma linha com ponto inicial em p e com o vetor modulo/direcao d. A equação retorna os pontos de
    * intersecao s e t.
    *
    * verificacao de colisao segundo livro Mathematics for 3D Game Programming
    * and Computer Graphics - Por Eric Lengyel - Pagina 119 - 120
    * Foi utilizado a substituicao da formula da reta sobre a da circunferencia
    * o que gera uma equaçao de segundo grau, assim verifica-se se a mesma possui raizes
    * Se possuir raizes a reta poderá estar tocando a circunferencia, porem precisamos
    * determinar se os pontos de interseccao pertencem a reta
    *
    * Autor: Henrique Vicentini e Fernando Bevilacqua
    */
   bool intersects(Circle &circ, float &s, float &t)
   {
      float a, b, c, r2, delta;
      // é subtraido o centro da circunferencia para levar a circunferencia a origem
      Vector centro(p.x - circ.p.x,  p.y - circ.p.y);

      // o raio ao quadrado da circunferencia
      r2 = circ.r * circ.r;

      // a da quadrica
      a = d.x*d.x + d.y*d.y;
      // b da quadrica
      b = 2*(centro.x*d.x + centro.y*d.y);
      // c da quadrica
      c = centro.x*centro.x + centro.y*centro.y - r2;

      // calcula o delta da quadrica
      delta = b*b - 4*a*c;

      // se negativo nao colide em hipotese alguma
      if( delta < 0 )
         return false;
      else
      {
         // calcula os pontos de interseccao
         s = ((-b)+sqrt(delta)) / (2*a);
         t = ((-b)-sqrt(delta)) / (2*a);

         if( (t > 0 && t < 1) || (s > 0 && s < 1) )
            return true;
      }
      return false;
   }


   inline void setPos(Vector _p)
   {
      p = _p;
   }

   inline void setDir(Vector _d)
   {
      d = _d;
   }

   void render()
   {
      Vector pf;
      pf = p + d;
      glBegin(GL_LINES);
         glVertex2f(p.x, p.y);
         glVertex2f(pf.x, pf.y);
      glEnd();
   }

   //plota o ponto de intersecao em funcao de s.
   //b indica se houve ou nao intesecao.
   void renderIntersection(float s, bool b)
   {
      if (b == false )
         return;

      //calcula o ponto de intesecao na linha.
      Vector p1;
      p1 = p + d*s;

      glPointSize(10);
      glBegin(GL_POINTS);
         glVertex2f(p1.x, p1.y);
      glEnd();
   }
};

#endif
