#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>

#include <plotOpenCv.h>
#include "dft.h"
#include "IIR.h"
#include "FIR.h"


using namespace cr::utils;


void audiotest()
{

    // Load the sound file
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("test.wav")) {
        // Error handling
        return;
    }

    // Get the audio samples(
    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();
    double samplingRate = buffer.getSampleRate();

    // Copy samples to a vector
    std::vector<int16_t> audioSamples(samples, samples + sampleCount);
    std::vector<int16_t> noisySamples;

    // Seed for random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Add noise to the audio samples
    for (auto& sample : audioSamples) 
    {
        // Adding a random value to each sample
        // Adjust the factor to control the amount of noise
        noisySamples.push_back( sample + rand() % 1000 - 500);
    }

    // if you dont know the frequencies inside ur test audio, you can use dft here
    // but dont forgot dft is heavy computationally which means try use really short test file around few seconds :(
    //std::vector<double> freqs = computeDFT(audioSamples, samplingRate);


    IIR iirFilter(samplingRate, 2500);
    FIR firFilter(samplingRate, 2000);
      
    std::vector<double> filteredSignalIIR;
    std::vector<double> filteredSignalFIR;

    for (auto element : noisySamples)
    {
        double value = iirFilter.processSignall(element);
        filteredSignalIIR.push_back(value);

        value = firFilter.processSignall(element);
        filteredSignalFIR.push_back(value);
    }

    Plot graph("Test graph", 1280, 720, cv::Scalar(0, 128, 128), cv::Scalar(50, 50, 50));
    for (int i = 60000; i < audioSamples.size() - 1280; i += 1)
    {
        graph.clean();
        graph.addPlot(audioSamples, 0, i, i + 300, cv::Scalar(255, 0, 0), 3);
        graph.addPlot(noisySamples, 1, i, i + 300, cv::Scalar(0, 255, 0), 1);
        graph.addPlot(filteredSignalIIR, 2, i, i + 300, cv::Scalar(0, 0, 255), 2);
        //graph.addPlot(filteredSignalFIR, 3, i, i + 300, cv::Scalar(100, 100, 100), 2);
        graph.show();
        cv::waitKey(50); 
    }

    // Create a new sound buffer from the vector
    sf::SoundBuffer bufferOriginal;
    if (!bufferOriginal.loadFromSamples(reinterpret_cast<const sf::Int16*>(audioSamples.data()), sampleCount, buffer.getChannelCount(), buffer.getSampleRate())) {
        // Error handling
        return;
    }
    // Play the sound
    sf::Sound soundOriginal(bufferOriginal);
    soundOriginal.play();
    
    // Wait until the sound is finished
    while (soundOriginal.getStatus() == sf::Sound::Playing)
    {
        // You can do some processing here
        sf::sleep(sf::milliseconds(100));
    }
    
    // Create a new sound buffer from the vector
    sf::SoundBuffer bufferNoisy;
    if (!bufferNoisy.loadFromSamples(reinterpret_cast<const sf::Int16*>(noisySamples.data()), sampleCount, buffer.getChannelCount(), buffer.getSampleRate()))
    {
        // Error handling
        return;
    }
    // Play the sound
    sf::Sound soundNoisy(bufferNoisy);
    soundNoisy.play();
    
    // Wait until the sound is finished
    while (soundNoisy.getStatus() == sf::Sound::Playing)
    {
        // You can do some processing here
        sf::sleep(sf::milliseconds(100));
    }
    
    // lets prepare int16_t buffer for playing from filtered data.
    std::vector<int16_t> filteredPlay;
    for (auto& element : filteredSignalIIR)
    {
        filteredPlay.push_back((int16_t)element);
    }
    
    // Create a new sound buffer from the vector
    sf::SoundBuffer bufferFiltered;
    if (!bufferFiltered.loadFromSamples(reinterpret_cast<const sf::Int16*>(filteredPlay.data()), sampleCount, buffer.getChannelCount(), buffer.getSampleRate())) {
        // Error handling
        return;
    }
    // Play the sound
    sf::Sound soundFiltered(bufferFiltered);
    soundFiltered.play();
    
    // Wait until the sound is finished
    while (soundFiltered.getStatus() == sf::Sound::Playing)
    {
        // You can do some processing here
        sf::sleep(sf::milliseconds(100));
    }

}


void artificallSignallTest()
{
    const double samplingRate = 1000.0; // 100 Hz
    const double signalFrequency = 3.0; // 3 Hz
    const int N = 10000; // Number of samples

    // Create the test signal with 3 Hz frequency
    std::vector<double> testSignal(N);
    for (int i = 0; i < N; ++i)
    {
        double time = i / samplingRate;
        testSignal[i] = 10 * cos(2 * PI * signalFrequency * time);
    }

    // Create second signal with 40Hz frequency
    const double signalFrequency2 = 40.0;
    std::vector<double> testSignal2(N);
    for (int i = 0; i < N; ++i)
    {
        double time = i / samplingRate;
        testSignal2[i] = 10 * cos(2 * PI * signalFrequency2 * time);
    }

    // final signal
    std::vector<double> combinedSignal;
    for (int i = 0; i < N; ++i)
    {
        double sum = testSignal.at(i) + testSignal2.at(i);
        combinedSignal.push_back(sum);
    }

    // lets add some noise to signal.
    for (auto& element : combinedSignal)
    {
        element += rand() % 10;
    }

    // Compute the DFT
    //std::vector<double> freqs = computeDFT(combinedSignal, samplingRate);

    IIR iirFilter(samplingRate, 60);
    FIR firFilter(samplingRate, 60);

    std::vector<double> filteredSignalIIR;
    std::vector<double> filteredSignalFIR;

    for (auto element : combinedSignal)
    {
        double value = iirFilter.processSignall(element);
        filteredSignalIIR.push_back(value);

        value = firFilter.processSignall(element);
        filteredSignalFIR.push_back(value);
    }

    Plot graph("Test graph", 1280, 720, cv::Scalar(0, 128, 128), cv::Scalar(50, 50, 50));
    for (int i = 0; i < combinedSignal.size() - 1280; ++i)
    {
        graph.clean();
        graph.addPlot(combinedSignal, 0, i, i + 500, cv::Scalar(255, 0, 0), 3);
        graph.addPlot(filteredSignalIIR, 1, i, i + 500, cv::Scalar(0, 255, 0), 1);
        graph.addPlot(filteredSignalFIR, 2, i, i + 500, cv::Scalar(0, 0, 255), 1);

        graph.show();
        cv::waitKey(0);
    }
}

int main()
{
    // filtering test with real audio signal
    audiotest();

    // filtering test with artificial signal
    artificallSignallTest();

    return 0;
}