//*********************************************************
//
// classe para fazer o gerenciamento de um tanque de guerra.
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//
//
//**********************************************************

#ifndef ___TANK__H___
#define ___TANK__H___

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#include "global.h"

#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"

#define RECT_SIZE 70


class Tank
{
private:
    Vector pos;
    Vector dir;
    float ang;

    int tex_id_tank, tex_id_tiro;
    int id;
    Vector ref;

    bool girando;
    float ja_girado;

    float tempo_disparo;

    int shots;
    int state; //0=off, 1=waiting, 2=shot, 3=exploding


    Timer t1, t2;
    Sprite *s1, *s2;

public:
   Tank(char *Name, int id_tank, int id_tiro, int _id)
   {
        tex_id_tank = id_tank;
        tex_id_tiro = id_tiro;
        id = _id;

        ang = rand()%100;

        ref.set(0,1);

        girando = false;
        ja_girado = 0;
        shots = 0;
        state = 0;

        s1 = new Sprite("tiro", textureID[2], 4, 30 );
        s2 = new Sprite("explosao", textureID[2], 1, 100 );

   }

   void render()
   {
        if( pos.x < 50 || pos.x > 700 || pos.y < 70 || pos.y > 700 )
        {
           girando = true;
        }

        gira();

        float x = cos(ang);
        float y = sin(ang);
        dir.set(x,y);
        //dir.normalize();


        if( rand()%1000 == 10 && girando == false && state == 0 )
        {
            state = 1;
            shots++;
            t1.start();
        }

        if( girando == false && state == 0 )
        {
             pos.x += dir.x*g_speed;
             pos.y += dir.y*g_speed;
        }

        glTranslated(pos.x, pos.y,0);

        shot(g_z++);

        char str[100];
        sprintf(str,"Tank %d", id);
        text(-30, -50, str);
        sprintf(str,"Shots: %d", shots);
        text(-30, -70,  str);

        glRotatef(ang*57.2957795130+180, 0, 0, 1);

        glBindTexture( GL_TEXTURE_2D, tex_id_tank );
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(-RECT_SIZE, RECT_SIZE, g_z);

            glTexCoord2f(1, 0);
            glVertex3f(RECT_SIZE, RECT_SIZE, g_z);

            glTexCoord2f(1, 1);
            glVertex3f(RECT_SIZE, -RECT_SIZE, g_z);

            glTexCoord2f(0, 1);
            glVertex3f(-RECT_SIZE, -RECT_SIZE, g_z);
        glEnd();
        g_z++;
   }


   void shot(int z)
   {
        if( state == 0 )
           return;

        if( t1.getTime() > 4000 )
        {
            state = 0;
        }

        if( t1.getTime() > 1000 && state == 1 )
        {
            state=2;
            s1->setPos(pos.x+dir.x*70.0, pos.y+dir.y*70.0);
            s1->start();
        }

        if( t1.getTime() > 1800  &&  state == 2 )
        {
            state=3;
            s2->setPos(pos.x+dir.x*370.0, pos.y+dir.y*370.0);
            s2->start();
        }

        s1->render(g_z++);
        s2->render(g_z++);

        glDisable(GL_TEXTURE_2D);
        glColor3f(1,0,1);
        glLineWidth(2);
        glBegin(GL_LINES);
            glVertex3f(dir.x*70.0, dir.y*70.0, g_z);
            glVertex3f(dir.x*300.0, dir.y*300.0, g_z++);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glColor3f(1,1,1);
   }

    void gira()
    {
        if( girando == false )
           return;

        ang += g_speed/20.0;
        ja_girado += g_speed/20.0;

        if( ja_girado > 3)
        {
           girando = false;
           ja_girado = 0;
        }
    }

   void setPos(float x, float y)
   {
       pos.x = x;
       pos.y = y;
   }

   void setRotation(float a)
   {
      ang = a;
   }

    void text(int x, int y, char *t)
    {
        int tam = (int)strlen(t);
        int c;
        glColor3f(0,0,1);
        glDisable(GL_TEXTURE_2D); //deve-se desabilitar a textura_2D para nao misturar com a cor do texto
        for(c=0; c<tam; c++)
        {
          glRasterPos3i(x + c*10, y, 20);
          glutBitmapCharacter(GLUT_BITMAP_8_BY_13, t[c]);
        }
        //deve-se restaurar a cor branca (default) para o blending com a textura nao mudar a cor dela
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
    }
};

#endif
