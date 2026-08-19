/**
*   Programa para ilustrar:
*   - iluminacao com GLSL no espaco do olho e no espaco do objeto
*   - calculo de angulo entre vetores

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 12/2012
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "glsl.h"
#include "quadrado.h"
#include "global.h"
#include "util.h"

#define LIGHT_POS_Y 0.1

//variaveis uniform
GLint loc_u_light;   //local da variavel bright do arquivo tex.vert
GLint loc_u_mode;    //local da variavel mode no arquivo tex.vert

float light_position[] = {0.0, LIGHT_POS_Y, 0.0, 1.0 }; //w tem que ser 1.

Glsl *shader1;
int mode = EYE; //modo de iluminacao: Olho, objeto ou via GL nativo

void init_gl(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glShadeModel (GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, 1, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,10,0,
              0,0,0,
              0,0,-1);//up

    //matrizGL();

    //deve ser chamado apos o glutLookAt() para que ja seja convertido para coordenadas do olho.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    //muda a intensidade da cor de cada pixel por uma senoide no intervalo [0.2, 4.2]
    glUniform3f(loc_u_light, light_position[0], light_position[1], light_position[2]);
    glUniform1i(loc_u_mode, mode);

    quadrado(mode);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if( key == 'e')
    {
        mode = EYE;
        shader1->setActive(true);
        printf("\nEYE ");
    }
    if( key == 'w')
    {
        mode = WORLD;
        shader1->setActive(true);
        printf("\nWORLD ");
    }
    //renderiza sem shader
    if( key == 'g')
    {
        mode = GL;
        shader1->setActive(false);
        printf("\nGL_EYE - sem shader ");
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (500, 300);
    glutCreateWindow("Iluminacao Espaco olho e espaco do mundo. Use: w, e, g");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    shader1 = new Glsl("glsl_6_Light\\src\\tex.vert", "glsl_6_Light\\src\\tex.frag");
    shader1->setActive(true);

    init_gl();

    loc_u_light = shader1->getUniformLoc("lightSourcePos");
    loc_u_mode  = shader1->getUniformLoc("eye_world"); //define iluminacao no espaco do olho ou do objeto

    glutMainLoop();
    return 0;
}
