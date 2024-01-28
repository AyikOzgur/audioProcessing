#pragma once

#include"Complex.h"
#include<vector>

std::vector<double> computeDFT(const std::vector<int16_t>& inputSignal, double samplingRate, int numberOfFreqs = 10);