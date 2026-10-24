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
int screenWidth = 1280, screenHeight = 720;


int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool shouldApplyGammaCorrection = false;

constexpr int StepsPerSample = 100; // frames de animacao entre duas amostras consecutivas do sinal

// '1': CDFT - uma cadeia de epicycles sobre o sinal complexo x + jy
// '2': DFT  - duas cadeias, uma para o vetor x e outra para o vetor y (fase do eixo y + pi/2)
bool useCdft = true;

std::vector<Epicycle> cdftEpicycles{};
std::vector<Epicycle> dftEpicyclesX{};
std::vector<Epicycle> dftEpicyclesY{};
std::deque<Vector2> trail{}; // ultimas posicoes da ponta, equivalente a um periodo completo
int signalSize = 0;
double t = 0.0;


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

    Vector2 tip;
    CV::color(0, 0, 0);
    if (useCdft)
    {
        tip = drawEpicycles(cdftEpicycles, Vector2(screenWidth / 2, screenHeight / 2), t);
    }
    else
    {
        // A cadeia de x fica no topo e a de y na lateral. Cada uma so contribui com uma coordenada da ponta.
        Vector2 tipX = drawEpicycles(dftEpicyclesX, Vector2(screenWidth / 2, 100), t);
        Vector2 tipY = drawEpicycles(dftEpicyclesY, Vector2(100, screenHeight / 2), t);
        tip.set(tipX.x, tipY.y);

        CV::color(0.7f, 0.7f, 0.7f);
        CV::line(tipX, tip);
        CV::line(tipY, tip);
    }

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
      case '1':
         useCdft = true;
         trail.clear();
         break;
      case '2':
         useCdft = false;
         trail.clear();
         break;

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

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Fourier - Epicycles");

   // Sinal de entrada: curva de um coracao com 64 amostras, centrada na origem (y cresce para baixo na canvas).
   signalSize = 64;
   std::vector<std::complex<double>> signal;
   std::vector<double> xs, ys;
   for (int n = 0; n < signalSize; n++)
   {
      double a = 2.0 * pi * n / signalSize;
      double x = 10.0 * 16.0 * pow(sin(a), 3);
      double y = -10.0 * (13.0 * cos(a) - 5.0 * cos(2 * a) - 2.0 * cos(3 * a) - cos(4 * a));
      signal.push_back(std::complex<double>(x, y));
      xs.push_back(x);
      ys.push_back(y);
   }

   cdftEpicycles = createEpicycles(complexDiscreteFourierTransform(signal));
   dftEpicyclesX = createEpicycles(discreteFourierTransform(xs));
   dftEpicyclesY = createEpicycles(discreteFourierTransform(ys), pi / 2);

   CV::run();
}
