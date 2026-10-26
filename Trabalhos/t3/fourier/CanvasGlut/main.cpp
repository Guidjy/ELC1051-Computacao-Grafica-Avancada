/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         08/2025
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.1
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
//    Programa cheio de numeros magicos. Nao use isso nunca.
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "fourier.h"
#include <deque>


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1600, screenHeight = 900;


int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool shouldApplyGammaCorrection = false;

<<<<<<< HEAD
std::list<Epicycle> epicycles{};
std::vector<Vector2> drawLinePoints{};
=======
constexpr int StepsPerSample = 100; // frames de animacao entre duas amostras consecutivas do sinal

// A tela e dividida ao meio: CDFT a esquerda, DFT a direita.
// CDFT: uma cadeia de epicycles sobre o sinal complexo x + jy.
// DFT:  duas cadeias, uma para o vetor x (topo) e outra para o vetor y (lateral), com fase do eixo y + pi/2.
std::vector<Epicycle> cdftEpicycles{};
std::vector<Epicycle> dftEpicyclesX{};
std::vector<Epicycle> dftEpicyclesY{};
std::deque<Vector2> cdftTrail{}; // ultimas posicoes da ponta, equivalente a um periodo completo
std::deque<Vector2> dftTrail{};
int signalSize = 0;
>>>>>>> 960247109b56a5d8002469f5b509dc19c3e5b484
double t = 0.0;

// Acrescenta a ponta atual ao traco, limitado a um periodo, e desenha o traco.
void drawTrail(std::deque<Vector2>& trail, Vector2 tip)
{
    trail.push_back(tip);
    if (trail.size() > signalSize * StepsPerSample)
    {
        trail.pop_front();
    }

    CV::color(1, 0, 0);
    for (size_t i = 1; i < trail.size(); i++)
    {
        CV::line(trail[i - 1], trail[i]);
    }
}


//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa funcao com poucas linhas de codigo.
void render()
{
    // t percorre um periodo (2pi) em signalSize amostras
    t += 2.0 * pi / signalSize / StepsPerSample;
    if (t >= 2.0 * pi)
    {
        t -= 2.0 * pi;
    }

    CV::color(1, 1, 1);
    CV::rectFill(0, 0, screenWidth, screenHeight);

<<<<<<< HEAD
	CV::color(0, 0, 0);
	auto endpoint = drawEpicycles(epicycles, epicycles.front().center, t);
	drawLinePoints.push_back(endpoint);
	for (auto point : drawLinePoints) CV::point(point);
=======
    float middleY = screenHeight / 2;
    float leftX = screenWidth / 4;
    float rightX = 3 * screenWidth / 4;
>>>>>>> 960247109b56a5d8002469f5b509dc19c3e5b484

    CV::color(0.7f, 0.7f, 0.7f);
    CV::line(screenWidth / 2, 0, screenWidth / 2, screenHeight);

    // CDFT
    CV::color(0, 0, 0);
    Vector2 cdftTip = drawEpicycles(cdftEpicycles, Vector2(leftX, middleY), t);
    drawTrail(cdftTrail, cdftTip);

    // DFT: cada cadeia so contribui com uma coordenada da ponta.
    CV::color(0, 0, 0);
    Vector2 tipX = drawEpicycles(dftEpicyclesX, Vector2(rightX, 120), t);
    Vector2 tipY = drawEpicycles(dftEpicyclesY, Vector2(screenWidth / 2 + 120, middleY), t);
    Vector2 dftTip(tipX.x, tipY.y);

    CV::color(0.7f, 0.7f, 0.7f);
    CV::line(tipX, dftTip);
    CV::line(tipY, dftTip);
    drawTrail(dftTrail, dftTip);

    // Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 27:
         exit(0);


   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( state == 0 ) //clicou
   {
   }
}

void desenhaEstrela()
{
    // Sinal de entrada: contorno de uma estrela de 5 pontas centrada na origem (y cresce para baixo na canvas).
    // Os 10 vertices alternam entre o raio externo e o interno, e cada aresta recebe SamplesPerEdge amostras.
    const int Vertices = 10;
    const int SamplesPerEdge = 8;
    const double OuterRadius = 250.0;
    const double InnerRadius = OuterRadius * 0.382; // razao que torna a estrela regular

    std::vector<Vector2> vertices;
    for (int v = 0; v < Vertices; v++)
    {
        double radius = (v % 2 == 0) ? OuterRadius : InnerRadius;
        double a = -pi / 2 + v * 2.0 * pi / Vertices; // primeira ponta para cima
        vertices.push_back(Vector2(radius * cos(a), radius * sin(a)));
    }

    signalSize = Vertices * SamplesPerEdge;
    std::vector<std::complex<double>> signal;
    std::vector<double> xs, ys;
    for (int n = 0; n < signalSize; n++)
    {
        Vector2 from = vertices[n / SamplesPerEdge];
        Vector2 to = vertices[(n / SamplesPerEdge + 1) % Vertices];
        double u = (n % SamplesPerEdge) / (double)SamplesPerEdge;
        double x = from.x + u * (to.x - from.x);
        double y = from.y + u * (to.y - from.y);
        signal.push_back(std::complex<double>(x, y));
        xs.push_back(x);
        ys.push_back(y);
    }

    cdftEpicycles = createEpicycles(complexDiscreteFourierTransform(signal));
    dftEpicyclesX = createEpicycles(discreteFourierTransform(xs));
    dftEpicyclesY = createEpicycles(discreteFourierTransform(ys), pi / 2);
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Fourier - Epicycles");

   desenhaEstrela();

   CV::run();
}
