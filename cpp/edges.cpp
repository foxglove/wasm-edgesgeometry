#include <array>
#include <cstdint>
#include <math.h>
#include <tuple>
#include <unordered_map>

#include "edges.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace std {
namespace {

template <class T> inline void hash_combine(std::size_t &seed, T const &v) {
  seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
struct HashValueImpl {
  static void apply(size_t &seed, Tuple const &tuple) {
    HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
    hash_combine(seed, get<Index>(tuple));
  }
};

template <class Tuple> struct HashValueImpl<Tuple, 0> {
  static void apply(size_t &seed, Tuple const &tuple) {
    hash_combine(seed, get<0>(tuple));
  }
};
} // namespace

template <typename... TT> struct hash<std::tuple<TT...>> {
  size_t operator()(std::tuple<TT...> const &tt) const {
    size_t seed = 0;
    HashValueImpl<std::tuple<TT...>>::apply(seed, tt);
    return seed;
  }
};
} // namespace std

////////////////////////////////////////////////////////////////////////////////

constexpr float DEG2RAD = M_PI / 180;

using VertexKey = std::tuple<int32_t, int32_t, int32_t>;
using EdgeKey =
    std::tuple<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t>;

struct Vector3 {
  float x;
  float y;
  float z;
};

struct Triangle {
  Vector3 a;
  Vector3 b;
  Vector3 c;

  Vector3 &operator[](int index) {
    switch (index) {
    default:
    case 0:
      return a;
    case 1:
      return b;
    case 2:
      return c;
    }
  }
};

struct EdgeData {
  int index0;
  int index1;
  Vector3 normal;
  bool used;

  EdgeData(int index0_, int index1_, Vector3 &normal_)
      : index0(index0_), index1(index1_), normal(normal_), used(false) {}
};

void readTriangle(Triangle &triangle, const float positions[],
                  const std::array<int, 3> &indexArray) {
  int p0 = indexArray[0] * 3;
  int p1 = indexArray[1] * 3;
  int p2 = indexArray[2] * 3;

  triangle.a.x = positions[p0 + 0];
  triangle.a.y = positions[p0 + 1];
  triangle.a.z = positions[p0 + 2];

  triangle.b.x = positions[p1 + 0];
  triangle.b.y = positions[p1 + 1];
  triangle.b.z = positions[p1 + 2];

  triangle.c.x = positions[p2 + 0];
  triangle.c.y = positions[p2 + 1];
  triangle.c.z = positions[p2 + 2];
}

void getNormal(Vector3 &normal, const Triangle &triangle) {
  // ab = a - b
  float abx = triangle.a.x - triangle.b.x;
  float aby = triangle.a.y - triangle.b.y;
  float abz = triangle.a.z - triangle.b.z;
  // cb = c - b
  float cbx = triangle.c.x - triangle.b.x;
  float cby = triangle.c.y - triangle.b.y;
  float cbz = triangle.c.z - triangle.b.z;

  // cross product of cb and ab
  normal.x = cby * abz - cbz * aby;
  normal.y = cbz * abx - cbx * abz;
  normal.z = cbx * aby - cby * abx;

  // normalize
  float lengthSq =
      normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
  if (lengthSq > 0) {
    float length = 1.0f / std::sqrt(lengthSq);
    normal.x *= length;
    normal.y *= length;
    normal.z *= length;
  } else {
    normal.x = 0;
    normal.y = 0;
    normal.z = 0;
  }
}

float dot(const Vector3 &v0, const Vector3 &v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

VertexKey hashVertex(const Vector3 &v, float precision) {
  int32_t a = int32_t(std::round(v.x * precision));
  int32_t b = int32_t(std::round(v.y * precision));
  int32_t c = int32_t(std::round(v.z * precision));
  return std::make_tuple(a, b, c);
}

EdgeKey hashEdge(const VertexKey &v0, const VertexKey &v1) {
  return std::make_tuple(std::get<0>(v0), std::get<1>(v0), std::get<2>(v0),
                         std::get<0>(v1), std::get<1>(v1), std::get<2>(v1));
}

void doCreateEdges(std::vector<float> &output, const float *positions,
                   uint positionsLength, const uint *indices,
                   uint indicesLength, float thresholdAngle) {
  output.clear();

  float precision = std::pow(10, 4);
  float thresholdDot = std::cos(DEG2RAD * thresholdAngle);

  bool hasIndices = indices != nullptr && indicesLength > 0;
  uint indexCount = hasIndices ? indicesLength : positionsLength / 3;
  std::array<int, 3> indexArray;
  std::array<VertexKey, 3> hashes;

  Vector3 normal;
  Triangle triangle;

  std::unordered_map<EdgeKey, EdgeData> edgeData;
  for (int i = 0; i < indexCount; i += 3) {
    if (hasIndices) {
      indexArray[0] = indices[i + 0];
      indexArray[1] = indices[i + 1];
      indexArray[2] = indices[i + 2];
    } else {
      indexArray[0] = i + 0;
      indexArray[1] = i + 1;
      indexArray[2] = i + 2;
    }

    readTriangle(triangle, positions, indexArray);
    getNormal(normal, triangle);
    Vector3 &a = triangle.a;
    Vector3 &b = triangle.b;
    Vector3 &c = triangle.c;

    // Create unique keys for each vertex
    hashes[0] = hashVertex(a, precision);
    hashes[1] = hashVertex(b, precision);
    hashes[2] = hashVertex(c, precision);

    // Skip degenerate triangles
    if (hashes[0] == hashes[1] || hashes[1] == hashes[2] ||
        hashes[2] == hashes[0]) {
      continue;
    }

    // Iterate over every edge
    for (int j = 0; j < 3; j++) {
      // Get the first and next vertex making up the edge
      int jNext = (j + 1) % 3;
      VertexKey &vecHash0 = hashes[j];
      VertexKey &vecHash1 = hashes[jNext];
      Vector3 &v0 = triangle[j];
      Vector3 &v1 = triangle[jNext];

      EdgeKey hash = hashEdge(vecHash0, vecHash1);
      EdgeKey reverseHash = hashEdge(vecHash1, vecHash0);

      auto reverseIt = edgeData.find(reverseHash);
      if (reverseIt != edgeData.end() && !reverseIt->second.used) {
        // If we found a sibling edge add it into the vertex array if
        // it meets the angle threshold and mark the edge as used
        if (dot(normal, reverseIt->second.normal) <= thresholdDot) {
          output.push_back(v0.x);
          output.push_back(v0.y);
          output.push_back(v0.z);

          output.push_back(v1.x);
          output.push_back(v1.y);
          output.push_back(v1.z);
        }

        reverseIt->second.used = true;
      } else {
        auto forwardIt = edgeData.find(hash);
        if (forwardIt == edgeData.end()) {
          edgeData.try_emplace(hash, indexArray[j], indexArray[jNext], normal);
        }
      }
    }
  }

  for (const auto &[key, entry] : edgeData) {
    if (entry.used) {
      continue;
    }

    int positionIndex0 = entry.index0 * 3;
    output.push_back(positions[positionIndex0 + 0]);
    output.push_back(positions[positionIndex0 + 1]);
    output.push_back(positions[positionIndex0 + 2]);

    int positionIndex1 = entry.index1 * 3;
    output.push_back(positions[positionIndex1 + 0]);
    output.push_back(positions[positionIndex1 + 1]);
    output.push_back(positions[positionIndex1 + 2]);
  }
}
