/*******************************************************************
*	ObjLoader Example
*
*	Descrição: Exemplo de utilização do ObjLoader. Neste exemplo é
*	utilizada a biblioteca GLFW para o gerenciamento da janela e das
*	entradas.
*
*	Autor: Alex Thomas Almeida Frasson
*
*	Universidade Federal de Santa Maria - 2012/1
*******************************************************************/

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "MyMesh.h"


#define WIDTH 1280
#define HEIGHT 720


MyMesh mesh;
float angle;			//Angulo de rotação do modelo.
float camZPos;			//Posição da camera no eixo z. (zoom)
int mouseX;				//Posição do mouse no eixo x.
bool mouseLeftPressed;	//Botão esquerdo do mouse, pressionado ou não.
GLFWwindow* window;



void render(void);
void cursorPosCallback(GLFWwindow *w, double x, double y);
void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods);
void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset);
void init(void);
void close(void);



int main()
{

	init();

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) == 0)
	{

		render();

	}

	close();

	return 0;
}

void render(void)
{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushMatrix();
		glTranslatef(0.0f, -1.0f, camZPos);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		mesh.draw();						//Desenha o modelo.
	glPopMatrix();


	glFlush();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void init(void)
{

	//GLFW
	if (!glfwInit())
		exit(1);
	window = glfwCreateWindow(WIDTH, HEIGHT, "ObjLoader", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallBack);

	//OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIDTH, HEIGHT);
	gluPerspective(54.0f, (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 1000.0f);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat lightPosition[] = {0.0, 3.0, 5.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Variables
	angle = 0.0f;
	camZPos = -20.0f;
	//Carrega o modelo
	//mesh.load("resources\\mp40.obj");
	//mesh.load("resources\\magequads.obj");
	//mesh.load("resources\\AstonMartin.obj");
	mesh.load("resources\\knight.obj");
	//mesh.load("resources\\assassin.obj");
}

void cursorPosCallback(GLFWwindow *w, double x, double y)
{
	if(mouseLeftPressed)
	{
		angle -= (mouseX - x)*0.1f;
		if(angle < 0.0f)
			angle = 360.0f;
		else if(angle > 360.0f)
			angle = 0.0f;
	}
	mouseX = x;
}

void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods)
{
	mouseLeftPressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
}

void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset)
{
	camZPos -= yoffset*camZPos/10.0;
	if(camZPos > -1)
		camZPos = -1;
}

void close(void)
{
	glfwTerminate();
}
