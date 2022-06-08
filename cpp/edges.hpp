#pragma once

#include <vector>

using uint = unsigned int;

void doCreateEdges(std::vector<float> &output, const float *positions,
                   uint positionsLength, const uint *indices,
                   uint indicesLength, float thresholdAngle);
