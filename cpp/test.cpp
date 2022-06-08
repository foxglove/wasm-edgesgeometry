#include <iostream>

#include "edges.hpp"

void a() {
  // clang-format off
  float positions[] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
  };
  uint indices[] = {
    0, 1, 2,
    0, 2, 3,
  };
  // clang-format on
  float thresholdAngle = 40.0f;

  uint positionsLength = sizeof(positions) / sizeof(positions[0]);
  uint indicesLength = sizeof(indices) / sizeof(indices[0]);
  std::vector<float> output;
  doCreateEdges(output, positions, positionsLength, indices, indicesLength,
                thresholdAngle);
  std::cout << "adjoined triangle edges=" << (output.size() / 6) << "\n";
}

void b() {
  // clang-format off
  float positions[] = {
    -1.0,-1.0,-1.0,
    -1.0,-1.0, 1.0,
    -1.0, 1.0, 1.0,

     1.0, 1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0,-1.0,

     1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,
     1.0,-1.0,-1.0,

     1.0, 1.0,-1.0,
     1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,

    -1.0,-1.0,-1.0,
    -1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,

     1.0,-1.0, 1.0,
    -1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,

    -1.0, 1.0, 1.0,
    -1.0,-1.0, 1.0,
     1.0,-1.0, 1.0,

     1.0, 1.0, 1.0,
     1.0,-1.0,-1.0,
     1.0, 1.0,-1.0,

     1.0,-1.0,-1.0,
     1.0, 1.0, 1.0,
     1.0,-1.0, 1.0,

     1.0, 1.0, 1.0,
     1.0, 1.0,-1.0,
    -1.0, 1.0,-1.0,

     1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,
    -1.0, 1.0, 1.0,

     1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
     1.0,-1.0, 1.0
  };
  // clang-format on

  float thresholdAngle = 40.0f;

  uint positionsLength = sizeof(positions) / sizeof(positions[0]);
  std::vector<float> output;
  doCreateEdges(output, positions, positionsLength, nullptr, 0, thresholdAngle);
  std::cout << "cube edges=" << (output.size() / 6) << "\n";
}

int main() {
  a();
  b();
}
