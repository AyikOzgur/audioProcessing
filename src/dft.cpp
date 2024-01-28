#include"dft.h"
#include<iostream>
#include <numeric>
#include <algorithm>
#include <iterator>


// Function to compute the Discrete Fourier Transform (DFT)
std::vector<double> computeDFT(const std::vector<double>& inputSignal, double samplingRate)
{

    // prepare complex version of signal
    std::vector<Complex> inputComplex;
    for(auto& element: inputSignal)
    {
        inputComplex.push_back(Complex(element, 0));
    }
    

    int N = (int)inputComplex.size();
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

    std::vector<double> freqs;
    // Output the magnitude of the results
    for (int i = 0; i < N / 2; ++i)
    {
        // Output only the first half of the DFT output
        double magnitude = std::sqrt(outputSignal[i].real * outputSignal[i].real + outputSignal[i].imag * outputSignal[i].imag);
        //std::cout << "Frequency " << i * (samplingRate / N) << " Hz: Magnitude = " << magnitude << std::endl;
        freqs.push_back(magnitude);
    }

    std::vector<size_t> indices(freqs.size());

    // Fill indices with 0, 1, 2, ..., data.size() - 1
    std::iota(indices.begin(), indices.end(), 0);

    // Find indices of the highest 5 elements
    std::vector<size_t> highestIndices(4);
    std::partial_sort_copy(indices.begin(), indices.end(), highestIndices.begin(), highestIndices.end(),
        [&freqs](size_t i1, size_t i2) { return freqs[i1] > freqs[i2]; });

    std::vector<double> outFreqs;

    // Output the indices of the highest 5 elements
    for (size_t idx : highestIndices)
    {
        outFreqs.push_back(idx * (samplingRate / N));
        std::cout << "Freq: " << idx * (samplingRate / N) << ", Value: " << freqs[idx] << std::endl;
    }

    return outFreqs;
}