#include "GeometryPrimitives.h"

#include <glm/geometric.hpp>

namespace {

// Flat shading: all three vertices share the outward face normal.
void appendTriangle(std::vector<Vertex>& output,
                    const glm::vec3& a,
                    const glm::vec3& b,
                    const glm::vec3& c,
                    const glm::vec3& color)
{
    const glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
    output.push_back({a, normal, color});
    output.push_back({b, normal, color});
    output.push_back({c, normal, color});
}

void appendQuad(std::vector<Vertex>& output,
                const glm::vec3& a,
                const glm::vec3& b,
                const glm::vec3& c,
                const glm::vec3& d,
                const glm::vec3& color)
{
    appendTriangle(output, a, b, c, color);
    appendTriangle(output, a, c, d, color);
}

} // namespace

std::vector<Vertex> GeometryPrimitives::createCubeVertices()
{
    constexpr float s = 0.5f;
    std::vector<Vertex> data;
    data.reserve(36);

    appendQuad(data, {-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, {-s,  s,  s}, {0.93f, 0.30f, 0.32f}); // front
    appendQuad(data, { s, -s, -s}, {-s, -s, -s}, {-s,  s, -s}, { s,  s, -s}, {0.33f, 0.72f, 0.90f}); // back
    appendQuad(data, {-s, -s, -s}, {-s, -s,  s}, {-s,  s,  s}, {-s,  s, -s}, {0.35f, 0.80f, 0.59f}); // left
    appendQuad(data, { s, -s,  s}, { s, -s, -s}, { s,  s, -s}, { s,  s,  s}, {0.98f, 0.70f, 0.30f}); // right
    appendQuad(data, {-s, -s, -s}, { s, -s, -s}, { s, -s,  s}, {-s, -s,  s}, {0.70f, 0.50f, 0.88f}); // bottom
    appendQuad(data, {-s,  s,  s}, { s,  s,  s}, { s,  s, -s}, {-s,  s, -s}, {0.41f, 0.75f, 0.94f}); // top
    return data;
}

std::vector<Vertex> GeometryPrimitives::createPyramidVertices()
{
    // Square base in the XZ plane and a single centered apex.
    const glm::vec3 a{-0.65f, -0.50f, -0.65f}; // back left
    const glm::vec3 b{ 0.65f, -0.50f, -0.65f}; // back right
    const glm::vec3 c{ 0.65f, -0.50f,  0.65f}; // front right
    const glm::vec3 d{-0.65f, -0.50f,  0.65f}; // front left
    const glm::vec3 apex{0.0f, 0.75f, 0.0f};

    std::vector<Vertex> data;
    data.reserve(18); // 4 side triangles + 2 triangles in the square base

    // Counter-clockwise winding as viewed from outside the pyramid.
    appendTriangle(data, d, c, apex, {0.95f, 0.46f, 0.29f}); // front face
    appendTriangle(data, c, b, apex, {0.95f, 0.69f, 0.30f}); // right face
    appendTriangle(data, b, a, apex, {0.42f, 0.70f, 0.95f}); // back face
    appendTriangle(data, a, d, apex, {0.44f, 0.83f, 0.65f}); // left face
    appendQuad(data, a, b, c, d, {0.66f, 0.53f, 0.87f}); // square base, facing down

    return data;
}
