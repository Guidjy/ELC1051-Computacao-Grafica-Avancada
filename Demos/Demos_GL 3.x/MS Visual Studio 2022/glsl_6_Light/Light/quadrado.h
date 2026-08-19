#include <GL/glut.h>

#include "global.h"

//cria um quadrado com coordenadas (-1,-1) --> (1,1) no plano xz, com normal (0,1,0)
void quadrado(int mode)
{
    float step = 0.001;
    glNormal3f(0,1,0);

    glBegin(GL_QUADS);

    if( mode == GL) //refina o quadrado para poder iluminar melhor via pipeline fixo
    {
        for(float z = -1; z <= 1; z += step)
        {
            for(float x =- 1; x <= 1; x +=step )
            {
                glVertex3f(x,      0, z);
                glVertex3f(x+step, 0, z);
                glVertex3f(x+step, 0, z+step);
                glVertex3f(x,      0, z+step);
            }
        }
    }
    else
    {
        glVertex3f(-1, 0, -1);
        glVertex3f(1, 0,   -1);
        glVertex3f(1, 0,  1);
        glVertex3f(-1, 0, 1);
    }
    glEnd();
}
