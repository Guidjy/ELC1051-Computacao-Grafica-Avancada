/*******************************************************************
*	ObjLoader Example
*	
*	Descrição: Exemplo de utilização do ObjLoader. Neste exemplo é
*	utilizada a biblioteca GLFW para o gerenciamento da janela e das
*	entradas.
*
*	Autor: Alex Thomas Almeida Frasson
*   Adaptacao para FreeGLUT: Cesar Pozzer - 2025
*
*	Universidade Federal de Santa Maria - 2012/1
*******************************************************************/

#include <GL/glut.h>
#include <iostream>
#include "MyMesh.h"


#define WIDTH 1280
#define HEIGHT 720


MyMesh mesh;
float angle;			//Angulo de rotação do modelo.
float camZPos;			//Posição da camera no eixo z. (zoom)
int mouseX;				//Posição do mouse no eixo x.
bool mouseLeftPressed;	//Botão esquerdo do mouse, pressionado ou não.
bool running;



void render(void);
void mousePosCallback(int x, int y);
void mouseButtonCallback(int button, int state);
void mouseWheelCallBack(int pos);
void init(void);
void close(void);


void init()
{
	//OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIDTH, HEIGHT);
	gluPerspective(54.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0f, 1000.0f);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat lightPosition[] = { 0.0, 3.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Variables
	running = true;
	angle = 0.0f;
	camZPos = -20.0f;
	//Carrega o modelo
	//mesh.load("resources\\mp40.obj");
	//mesh.load("resources\\magequads.obj");
	//mesh.load("resources\\AstonMartin.obj");
	mesh.load("resources\\knight.obj");
	//mesh.load("resources\\assassin.obj");
}

//imprime as matrizes da pilha do opengl
void printMatrix(GLenum pname)
{
    GLfloat m[16]; //usado para leitura das matrizes
    glGetFloatv(pname, m);
    int l, c;
    printf("\n\n Matriz = %d", pname);
    for (l = 0; l < 4; l++)
    {
        printf("\n");
        for (c = 0; c < 4; c++)
        {
            int pos = l * 4 + c;
            printf(" %.2f", m[pos]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushMatrix();
	glTranslatef(0.0f, -1.0f, camZPos);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	mesh.draw();//Desenha o modelo.
	glPopMatrix();


	glFlush();
	glutSwapBuffers();

}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
    //printf("%c", key);
    key = tolower(key);
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    int  argc = 0;
    //char *argv[1] = {"teste"};
    glutInit(&argc, NULL);

    //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(450, 10);
    /* create window */
    glutCreateWindow("GluLookAt Demo");
    init();
    printf("Digite W para mudar o modo de visualizacao: wireframe/fill");

    glutDisplayFunc(display);
    //glutMotionFunc(MotionFunc);
    //glutMouseFunc(MouseFunc);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
