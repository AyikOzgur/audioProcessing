#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>

#include <plotOpenCv.h>
#include "dft.h"
#include "IIR.h"
#include "FIR.h"


using namespace cr::utils;


int main()
{
    const double samplingRate = 1000.0; // 100 Hz
    const double signalFrequency = 3.0; // 3 Hz
    const int N = 10000; // Number of samples

    // Create the test signal with 3 Hz frequency
    std::vector<double> testSignal(N);
    for (int i = 0; i < N; ++i) 
    {
        double time = i / samplingRate;
        testSignal[i] = 10*cos(2 * PI * signalFrequency * time);
    }

    // Create second signal with 40Hz frequency
    const double signalFrequency2 = 40.0;
    std::vector<double> testSignal2(N);
    for (int i = 0; i < N; ++i)
    {
        double time = i / samplingRate;
        testSignal2[i] = 10*cos(2 * PI * signalFrequency2 * time);
    }

    // final signal
    std::vector<double> combinedSignal;
    for (int i = 0; i < N; ++i)
    {
        double sum = testSignal.at(i) +testSignal2.at(i);
        combinedSignal.push_back(sum);
    }

    // lets add some noise to signal.
    for(auto& element : combinedSignal)
    {
        element += rand() % 10;
    }

    // Compute the DFT
    //std::vector<double> freqs = computeDFT(combinedSignal, samplingRate);

    IIR iirFilter(samplingRate, 60);
    FIR firFilter(samplingRate, 60);

    IIR iirCombined(samplingRate, 10);
    FIR firCombined(samplingRate, 10);

    std::vector<double> filteredSignalIIR;
    std::vector<double> filteredSignalFIR;
    std::vector<double> combinedFilter;

    for(auto element : combinedSignal)
    {
        double value = iirFilter.processSignall(element);
        filteredSignalIIR.push_back(value);

        value = firFilter.processSignall(element);
        filteredSignalFIR.push_back(value);


        // first FIR
        //value = iirCombined.processSignall(element);
        //value = firCombined.processSignall(value);
        //combinedFilter.push_back(value);
    }


    Plot graph("Test graph", 1280, 720, cv::Scalar(0, 128, 128), cv::Scalar(50, 50, 50));
    for (int i = 0; i < combinedSignal.size() - 1280; ++i)
    {
        graph.clean();
        graph.addPlot(combinedSignal, 0, i, i + 500, cv::Scalar(255, 0, 0), 3);
        graph.addPlot(filteredSignalIIR, 1, i, i + 500, cv::Scalar(0, 255, 0), 1);
        graph.addPlot(filteredSignalFIR, 2, i, i + 500, cv::Scalar(0, 0, 255), 1);
       // graph.addPlot(combinedFilter, 3, i, i + 1000, cv::Scalar(255, 255, 255), 1);

        graph.show();
        cv::waitKey(0); 
    }

    return 0;
}