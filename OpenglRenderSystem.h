#pragma once

#include "Vertex.h"

#include <array>
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

// OpenGL 3.3 renderer: programmable pipeline (VAO/VBO + GLSL shaders).
class OpenglRenderSystem {
public:
    OpenglRenderSystem() = default;
    ~OpenglRenderSystem();
    OpenglRenderSystem(const OpenglRenderSystem&) = delete;
    OpenglRenderSystem& operator=(const OpenglRenderSystem&) = delete;

    void init(); // Requires an active GLFW OpenGL context.
    void clearDisplay(float r, float g, float b) const;
    void setViewport(double x, double y, double width, double height) const;
    void renderTriangleSoup(const std::vector<Vertex>& vertices) const;
    void renderLines(const std::vector<Vertex>& vertices) const;

    void setupLight(uint32_t index, glm::vec3 position,
                    glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    void turnLight(uint32_t index, bool enable);

    void setWorldMatrix(const glm::mat4& matrix) { m_world = matrix; }
    void setViewMatrix(const glm::mat4& matrix) { m_view = matrix; }
    void setProjectionMatrix(const glm::mat4& matrix) { m_projection = matrix; }
    void setCameraPosition(const glm::vec3& position) { m_cameraPosition = position; }

    const glm::mat4& getWorldMatrix() const { return m_world; }
    const glm::mat4& getViewMatrix() const { return m_view; }
    const glm::mat4& getProjectionMatrix() const { return m_projection; }

private:
    struct Light {
        glm::vec3 position{0.0f};
        glm::vec3 ambient{0.0f};
        glm::vec3 diffuse{0.0f};
        glm::vec3 specular{0.0f};
        bool enabled = false;
    };

    void renderData(const std::vector<Vertex>& vertices, unsigned int mode) const;
    unsigned int m_program = 0;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    glm::mat4 m_world{1.0f};
    glm::mat4 m_view{1.0f};
    glm::mat4 m_projection{1.0f};
    glm::vec3 m_cameraPosition{0.0f, 0.0f, 3.0f};
    std::array<Light, 8> m_lights{};
};
