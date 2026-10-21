#include "fourier.h"
#include <math.h>

using namespace std::complex_literals;

std::vector<std::complex<double>> discreteFourierTransform(std::vector<double> x)
{
	int N = x.size();
	// Output vector of complex numbers representing the amplitude and fase of each frequency
	auto X = std::vector<std::complex<double>>(N);

	// Calculates each X[k]
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