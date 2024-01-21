#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>
#include <plotOpenCv.h>

using namespace cr::utils;

int main()
{

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("./test.wav"))
    {
        std::cout << "Could not load file" << std::endl;
        return -1;
    }

    const sf::Int16 *samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();
    uint32_t sampleRate = buffer.getSampleRate();
    std::cout << "rate : " << sampleRate << std::endl;
    std::cout << "number of samples : " << sampleCount << std::endl;
    // Create a sound source and attach the buffer to it
    sf::Sound sound;
    sound.setBuffer(buffer);

    Plot graph("Test graph", 1280, 720, cv::Scalar(0, 128, 128), cv::Scalar(50, 50, 50));

    //sampleCount = 722278;
    std::vector<int16_t> linePoints;
    try {
        linePoints.resize(sampleCount);
        std::copy(samples, samples + sampleCount, linePoints.begin());
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        // Handle memory allocation failure
    } catch (...) {
        std::cerr << "An unexpected error occurred\n";
        // Handle other exceptions
    }

    // Play the sound
    sound.play();
    for (int i = 0; i < sampleCount; i = i+1000)
    {
        //graph.clean();
        //graph.addPlot(linePoints, 0, 0 + i, 1000 + i, cv::Scalar(255, 0, 0), 2);
        //graph.show();
        //cv::waitKey(1);
    }
        graph.clean();
        graph.addPlot(linePoints, 0, 0 , 0, cv::Scalar(255, 0, 0), 2);
        graph.show();
        cv::waitKey(1);
    cv::waitKey(0);

    return 0;
}