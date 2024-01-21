#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdint> // Include for int16_t
#include <iostream>
#include <plotOpenCv.h>

using namespace cr::utils;

int main() {
    // Sound propertiesP
    const unsigned SAMPLE_RATE = 100; // Sample rate in Hz
    const float FREQUENCY = 5; // Frequency of the tone (A4)
    const double TONE_DURATION = 0.5; // Duration of the tone in seconds
    const double PAUSE_DURATION = 0.5; // Duration of the pause in seconds
    const unsigned NUM_CHANNELS = 1; // Number of audio channels (1 = mono, 2 = stereo)
    const unsigned REPEATS = 12; // Number of bips
    
    // Calculate the number of samples for the tone and the pause
    const unsigned NUM_SAMPLES_TONE = static_cast<unsigned>(SAMPLE_RATE * TONE_DURATION);
    const unsigned NUM_SAMPLES_PAUSE = static_cast<unsigned>(SAMPLE_RATE * PAUSE_DURATION);
    const unsigned TOTAL_SAMPLES = REPEATS * (NUM_SAMPLES_TONE + NUM_SAMPLES_PAUSE); // Total samples for all bips

    std::cout << "Total sample : " << TOTAL_SAMPLES << std::endl;
    // Create an array to hold the audio samples
    int16_t* rawAudioData = new int16_t[TOTAL_SAMPLES];

    // Generate the bips
    for (unsigned bip = 0; bip < REPEATS; ++bip) {
        unsigned start = bip * (NUM_SAMPLES_TONE + NUM_SAMPLES_PAUSE);
        for (unsigned i = 0; i < NUM_SAMPLES_TONE; ++i) {
            // Bip sound
            rawAudioData[start + i] = 30000 * sin(2 * M_PI * FREQUENCY * i / SAMPLE_RATE);
        }
        for (unsigned i = NUM_SAMPLES_TONE; i < NUM_SAMPLES_TONE + NUM_SAMPLES_PAUSE; ++i) {
            // Pause
            rawAudioData[start + i] = 0;
        }
    }

    // Create a sound buffer and load the raw data
    sf::SoundBuffer buffer;
    if (!buffer.loadFromSamples(rawAudioData, TOTAL_SAMPLES, NUM_CHANNELS, SAMPLE_RATE)) {
        std::cerr << "Loading failed!" << std::endl;
        return 1;
    }

    // Free the raw audio data array
    //delete[] rawAudioData;

    // Create a sound source and attach the buffer to it
    sf::Sound sound;
    sound.setBuffer(buffer);

    // Play the sound
    sound.play();

    Plot graph("Test graph", 1280, 720,cv::Scalar(0, 128, 128), cv::Scalar(50, 50, 50));

    std::vector<int16_t> linePoints(1280);

    std::copy(rawAudioData, rawAudioData + 1280, linePoints.begin());

    for(int i = 0; i< 1200; ++i)
    {
        graph.clean();
        graph.addPlot(linePoints,0, 0 + i , 100 + i, cv::Scalar(255,0,0), 2);
        graph.show();
        cv::waitKey(10);
    }
    cv::waitKey(0);

    // Don't close the application immediately, let the sound play
    sf::sleep(sf::seconds(REPEATS * (TONE_DURATION + PAUSE_DURATION)));

    return 0;
}
