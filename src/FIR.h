#include<math.h>

class FIR
{
public:
	FIR(double fs, double fc, int kernelSize = 16)
	{
		// Calculate normalized cutoff frequency
        m_inputs.reserve(kernelSize);
        m_impulseResponse.resize(kernelSize);

        double NUM_TOTAL_SAMPLES = kernelSize;
        double CUTOFF_FREQUENCY_HZ = fc;
        double SAMPLE_TIME_S = 1 / fs;
        double NUM_SHIFT_SAMPLES = 0;

        // calculate impulse response by using fs(sampling freq), fc(cut-off freq) and kernel size.

        for (int n = 0; n < NUM_TOTAL_SAMPLES; n++)
        {
            if (n != NUM_SHIFT_SAMPLES)
            {
                m_impulseResponse[n] = sin(2.0 * PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) / (PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            }
            else /* Avoid divide-by-zero, limit is 2*fc */
            {
                m_impulseResponse[n] = 2.0 * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S;
            }

        }


	}

	double processSignall(double x)
	{
        // Add new sample to the beginning of m_inputs
        m_inputs.insert(m_inputs.begin(), x);

        // Keep the size of m_inputs equal to the size of the filter
        if (m_inputs.size() > m_impulseResponse.size()) 
        {
            m_inputs.pop_back();
        }

        // Compute the filtered output
        double filteredOutput = 0.0;
        for (size_t i = 0; i < m_impulseResponse.size(); ++i) 
        {
            filteredOutput += m_impulseResponse[i] * m_inputs[i];
        }
        //std::cout << "Input : " << x << std::endl;
        //std::cout << "Filtered output : " << filteredOutput << std::endl;
        return filteredOutput;
	}

private:

	std::vector<double> m_impulseResponse;
	std::vector<double> m_inputs;

};