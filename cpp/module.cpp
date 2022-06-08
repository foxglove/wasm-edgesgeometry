#include <emscripten/bind.h>

#include "edges.hpp"

auto createEdges(uintptr_t positions, uint positionsLength, uintptr_t indices,
                 uint indicesLength, float thresholdAngle) {
  const float *positionsPtr = reinterpret_cast<const float *>(positions);
  const uint *indicesPtr = reinterpret_cast<const uint *>(indices);

  std::vector<float> output;
  doCreateEdges(output, positionsPtr, positionsLength, indicesPtr,
                indicesLength, thresholdAngle);
  return emscripten::val{
      emscripten::typed_memory_view(output.size(), output.data())};
}

EMSCRIPTEN_BINDINGS(wasm_edgesgeometry) {
  emscripten::function("createEdges", &createEdges);
}
