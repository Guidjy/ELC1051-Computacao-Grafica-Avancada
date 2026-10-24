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
            // e^(-jθ) = cos(θ) - jsin(θ)  (Euler's relation)
            std::complex<double> expTerm(std::cos(angle), -std::sin(angle));
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
            // e^(-jθ) = cos(θ) - jsin(θ)
            std::complex<double> expTerm(std::cos(angle), -std::sin(angle));
            // operator overloading from <complex> solves (Re_x + Im_x * j) * (cos - sin * j) automatically
            sum += x[n] * expTerm;
        }
        X[k] = sum / static_cast<double>(N);
    }

    return X;
}

std::vector<Epicycle> createEpicycles(const std::vector<std::complex<double>>& X, double phaseOffset)
{
    int N = X.size();
    auto epicycles = std::vector<Epicycle>();

    for (int k = 0; k < N; k++)
    {
        Epicycle epicycle{};
        epicycle.radius = std::abs(X[k]);
        epicycle.initialRotation = std::arg(X[k]) + phaseOffset;
        // k > N/2 equivale a frequencia negativa k - N (identica a k nas amostras, mas suave entre elas)
        epicycle.rotationSpeed = (k <= N / 2) ? k : k - N;

        epicycles.push_back(epicycle);
    }

    return epicycles;
}

Vector2 drawEpicycles(const std::vector<Epicycle>& epicycles, Vector2 center, double t)
{
    for (const Epicycle& epicycle : epicycles)
    {
        CV::circle(center, epicycle.radius, CircleDivs);

        double pointerAngle = epicycle.initialRotation + epicycle.rotationSpeed * t;
        Vector2 pointerEndpoint(center.x + epicycle.radius * std::cos(pointerAngle),
                                center.y + epicycle.radius * std::sin(pointerAngle));

        CV::line(center, pointerEndpoint);

        // O centro do proximo epicycle e a extremidade deste
        center = pointerEndpoint;
    }

    return center;
}
