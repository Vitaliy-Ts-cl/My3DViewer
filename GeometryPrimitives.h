#pragma once

#include "Vertex.h"
#include <vector>

namespace GeometryPrimitives {

// Six colored faces, twelve triangles (36 vertices).
std::vector<Vertex> createCubeVertices();

// Four triangular side faces and a square base (18 vertices).
std::vector<Vertex> createPyramidVertices();

} // namespace GeometryPrimitives
