#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>
#include <plotOpenCv.h>
#include "dft.h"

using namespace cr::utils;


int main()
{

    const double samplingRate = 100.0; // 100 Hz
    const double signalFrequency = 3.0; // 3 Hz
    const int N = 1000; // Number of samples

    // Create the test signal with 3 Hz frequency
    std::vector<double> testSignal(N);
    for (int i = 0; i < N; ++i) 
    {
        double time = i / samplingRate;
        testSignal[i] = cos(2 * PI * signalFrequency * time);
    }

    // Create second signal with 40Hz frequency
    const double signalFrequency2 = 40.0;
    std::vector<double> testSignal2(N);
    for (int i = 0; i < N; ++i)
    {
        double time = i / samplingRate;
        testSignal2[i] = cos(2 * PI * signalFrequency2 * time);
    }

    // final signal
    std::vector<double> combinedSignal;
    for (int i = 0; i < N; ++i)
    {
        double sum = testSignal.at(i) + testSignal2.at(i);
        combinedSignal.push_back(sum);
    }

    // Compute the DFT
    std::vector<Complex> dftOutput = computeDFT(combinedSignal);

    // Output the magnitude of the results
    for (int i = 0; i < N / 2; ++i) 
    { 
        // Output only the first half of the DFT output
        double magnitude = std::sqrt(dftOutput[i].real * dftOutput[i].real + dftOutput[i].imag * dftOutput[i].imag);
        std::cout << "Frequency " << i * (samplingRate / N) << " Hz: Magnitude = " << magnitude << std::endl;
    }
    Plot graph("Test graph", 1280, 720, cv::Scalar(0, 128, 128), cv::Scalar(50, 50, 50));

    
    for (int i = 0; i < combinedSignal.size() - 1280; ++i)
    {
        graph.clean();
        graph.addPlot(combinedSignal, 0, i, i + 100, cv::Scalar(255, 0, 0), 2);
        graph.show();
        cv::waitKey(0); 
    }
    return 0;
}