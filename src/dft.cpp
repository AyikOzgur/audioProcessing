#include"dft.h"

// Function to compute the Discrete Fourier Transform (DFT)
std::vector<Complex> computeDFT(const std::vector<double>& inputSignal)
{

    // prepare complex version of signal
    std::vector<Complex> inputComplex;
    for(auto& element: inputSignal)
    {
        inputComplex.push_back(Complex(element, 0);
    }
    

    int N = inputComplex.size();
    std::vector<Complex> outputSignal(N);

    for (int k = 0; k < N; ++k)
    {  // For each output element
        Complex sum;
        for (int t = 0; t < N; ++t)
        {  // Compute the sum of the input elements times the complex exponential
            double angle = 2 * PI * t * k / N;
            Complex exp(-sin(angle), cos(angle)); // e^(-2*pi*i*t*k/N)
            sum = sum + inputComplex[t] * exp;
        }
        outputSignal[k] = sum;
    }

    return outputSignal;
}