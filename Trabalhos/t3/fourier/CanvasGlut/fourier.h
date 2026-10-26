#pragma once

/*
A transformada de Fourier é uma ferramenta matemática utilizada para decomopor um sinal (qúe pode variar no tempo ou no espaço) 
nas suas frequências constituintes (senos e cossenos).
	- Transormada direta: converte o sinal do domínio do tempo/espaço para o domínio da frequência
	- Transformada inversa: reconstrói o sinal original a partir das suas frequências constituintes.

Discrete Fourier Transform (DFT): Transformada de Fourier aplicada a sinais discretos e periódicos.
	- Sinal de entrada: vetor com N amostras reais x[n] | x = 0, 1, ..., N - 1
	- Saída: um vetor X[k] de números complexos representando a amplitude e a fase ed cada frequência k

	DFT direta: X[k] = 1/N * Σ( x[n]e^(-j*(2πkn/N)) ) -> Pode ser separada em partes real (Re) e imaginária (Im)

	Dessa transformação, obtem-se para cada frequência k:
		- amplitude: α = √(Re² + Im²)
		- fase: Φ = atan2(Im, Re)

Complex Discrete Fourier Transform (CDFT): É a extensão da DFT para casos em que o sinal de entrada x[n] é composto por
números complexos. Em CG, um ponto 2D P = (x, y) pode ser mapeado como um número complexo z = x + jy, onde a parte real
(Re) representa a coordenada x e a parte imaginária (Im) representa a coordenada y.

	Ao aplicar a CDFT em uma sequência de pontos 2D (ex: polígino ou contorno de um desenho), obtem-se:
		- Re: parte real do coeficiente ed frequência
		- Im: parte imaginária do coeficiente de frequência
		- Fase: Φ = ângulo de rotação inicial
		- Amplitude: α = raio do círculo
		- Frequência: k = velocidade e sentido de rotação

Epicycles: Círculo cujo centro gira ao longo da circunferência de outro circulo. Pode ser utilizado para visualização da
transformada de Fourier:
	- Cada coeficiente de frequência gerada pela DFT/CDFT pode ser representado como um círculo
	  e um vetor giratório (ponteiro)
	- As propriedades extraídas da transformada mapeiam diretamente os parâmetros de cada epicycle:
		- Raio: aplitude α
		- Velocidade de rotação: frequência k
		- Rotação inicial: fase Φ
		- Centro: extremidade do epicycle anterior
*/


#include <complex>
#include <vector>
#include "Vector2.h"

constexpr double pi = 3.14159265358979323846;

std::vector<std::complex<double>> discreteFourierTransform(std::vector<double> x);

std::vector<std::complex<double>> complexDiscreteFourierTransform(std::vector<std::complex<double>> x);

typedef struct
{
    double radius;          // amplitude
    double initialRotation; // fase
    double rotationSpeed;   // frequencia com sinal (negativa = gira no sentido horario)
} Epicycle;

// Cria um epicycle por coeficiente de X. Indices k > N/2 representam frequencias negativas (k - N), o que
// faz a curva passar suavemente entre as amostras. phaseOffset e somado a fase de todos (ex: pi/2 para o eixo y na DFT).
std::vector<Epicycle> createEpicycles(const std::vector<std::complex<double>>& X, double phaseOffset = 0.0);

<<<<<<< HEAD
Vector2 drawEpicycles(std::list<Epicycle> epicycles, Vector2 center, double t);
=======
// Desenha a cadeia de epicycles a partir de center no instante t e retorna a extremidade do ultimo.
Vector2 drawEpicycles(const std::vector<Epicycle>& epicycles, Vector2 center, double t);
>>>>>>> 960247109b56a5d8002469f5b509dc19c3e5b484
