#pragma once
#include<math.h>

//const double PI = 3.141592653589793238460;


enum class FilterType 
{
	HIGH_PASS,
	LOW_PASS,
	BAND_PASS
};


class IIR
{

public:

	IIR(double fs, double fc, FilterType type = FilterType::LOW_PASS) 
	{
		double omega = 2 * PI * fc;

		double k = (2 * PI * fc) / (tan((PI * fc) / fs));

		double omegaSquare = omega * omega;
		double kSquare = k * k;

		b_0 = omegaSquare / (omegaSquare + kSquare + sqrt(2) * k * omega);
		b_1 = 2 * b_0;
		b_2 = b_0;

		a_1 = (2 * omegaSquare - 2 * kSquare) / (omegaSquare + kSquare + sqrt(2) * k * omega);
		a_2 = (omegaSquare + kSquare - sqrt(2) * k * omega) / (omegaSquare + kSquare + sqrt(2) * k * omega);
	}

	double processSignall(double x) 
	{
		// apply filter.
		double y = b_0 * x + b_1 * x_1 + b_2 * x_2 - a_1 * y_1 - a_2 * y_2;
	
		// update old outs and ints for next processing
		x_2 = x_1;
		x_1 = x;
		y_2 = y_1;
		y_1 = y;

		return y;
	}

private:

	/// Filter coefficients
	double b_0{0};
	double b_1{0};
	double b_2{0};
	double a_1{0};
	double a_2{0};

	/// Previous input and outputs
	double x_1{0};
	double x_2{0};
	double y_1{0};
	double y_2{0};

};