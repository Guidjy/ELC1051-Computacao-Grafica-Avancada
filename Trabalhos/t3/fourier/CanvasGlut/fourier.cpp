#include "fourier.h"
#include "gl_canvas2d.h"
#include <math.h>

using namespace std::complex_literals;

constexpr int CircleDivs = 20;

std::vector<std::complex<double>> discreteFourierTransform(std::vector<double> x)
{
	int N = x.size();
	// Output vector of complex numbers representing the amplitude and fase of each frequency
	auto X = std::vector<std::complex<double>>(N);

	// Calculates each X[k] coeficient
	for (int k = 0; k < N; k++)
	{
		// X[k] = 1/N * Σ( x[n]e^(-j*(2πkn/N)) )
		std::complex<double> sum = 0.0 + 0.0i;
		for (int n = 0; n < N; n++)
		{
			// 2πkn / N
			double angle = 2.0 * pi * k * n / N;
			// e^(j * -2πkn)
			std::complex<double> expTerm(std::cos(angle), -std::sin(angle));  // Euler's relation: e^(jθ) = cos(θ) - jsin(θ)
			// x[n] * e^(j * -2πkn)
			sum += x[n] * expTerm;
		}
		X[k] = sum / static_cast<double>(N);
	}

	return X;
}

std::vector<std::complex<double>> complexDiscreteFourierTransform(std::vector<std::complex<double>> x)
{
	int N = x.size();
	// Output vector of complex numbers representing the amplitude and phase of each frequency
	auto X = std::vector<std::complex<double>>(N);

	// Calculates each X[k] coeficient
	for (int k = 0; k < N; k++)
	{
		// X[k] = 1/N * Σ( x[n]e^(-j*(2πkn/N)) )
		std::complex<double> sum = 0.0 + 0.0i;
		for (int n = 0; n < N; n++)
		{
			// 2πkn / N
			double angle = 2.0 * pi * k * n / N;
			// e^(-j * 2πkn/N)
			std::complex<double> expTerm(std::cos(angle), -std::sin(angle));  // Euler's relation: e^(-jθ) = cos(θ) - jsin(θ)
			// x[n] * e^(-j * 2πkn/N) 0-0: operator overloading from <complex> solves (Re_x + Im_x * j) * (cos - sin * j) automatically
			sum += x[n] * expTerm;
		}
		X[k] = sum / static_cast<double>(N);
	}

	return X;
}

std::list<Epicycle> createEpicycles(Vector2 center, std::vector<std::complex<double>> X)
{
	auto epicycles = std::list<Epicycle>();

	for (int k = 0; k < X.size(); k++)
	{
		double re = X[k].real();
		double im = X[k].imag();

		double frequency = k;
		double amplitude = std::sqrt(pow(re, 2) + pow(im, 2));
		double phase = std::atan2(im, re);

		Epicycle epicycle{};
		epicycle.center = center;
		epicycle.radius = amplitude;
		epicycle.initialRotation = phase;
		epicycle.rotationSpeed = frequency;

		epicycles.push_back(epicycle);

		center.x += epicycle.radius * std::cos(epicycle.initialRotation);
		center.y += epicycle.radius * std::sin(epicycle.initialRotation);
	}

	return epicycles;
}

void drawEpicycles(std::list<Epicycle> epicycles, Vector2 center, double t)
{
	if (epicycles.empty())
	{
		return;
	}

	Epicycle epicycle = epicycles.front();
	epicycle.center = center;
	epicycles.pop_front();

	CV::circle(epicycle.center, epicycle.radius, CircleDivs);

	double pointerAngle = epicycle.initialRotation + epicycle.rotationSpeed * t;

	Vector2 pointerEndpoint = epicycle.center;
	pointerEndpoint.x += epicycle.radius * std::cos(pointerAngle);
	pointerEndpoint.y += epicycle.radius * std::sin(pointerAngle);

	CV::line(epicycle.center, pointerEndpoint);

	drawEpicycles(epicycles, pointerEndpoint, t);
}