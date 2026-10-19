#include <glad/glad.h> // Always load glad before including GLFW.

#include "OpenglRenderSystem.h"
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

const char* kVertexShader = R"GLSL(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vWorldPosition;
out vec3 vNormal;
out vec3 vColor;

void main() {
    vec4 worldPosition = uWorld * vec4(aPosition, 1.0);
    vWorldPosition = worldPosition.xyz;
    vNormal = mat3(transpose(inverse(uWorld))) * aNormal;
    vColor = aColor;
    gl_Position = uProjection * uView * worldPosition;
}
)GLSL";

const char* kFragmentShader = R"GLSL(
#version 330 core
in vec3 vWorldPosition;
in vec3 vNormal;
in vec3 vColor;
out vec4 FragColor;

uniform vec3 uCameraPosition;
uniform vec3 uLightPosition[8];
uniform vec3 uLightAmbient[8];
uniform vec3 uLightDiffuse[8];
uniform vec3 uLightSpecular[8];
uniform int uLightEnabled[8];

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(uCameraPosition - vWorldPosition);
    vec3 color = 0.10 * vColor;

    for (int i = 0; i < 8; ++i) {
        if (uLightEnabled[i] == 0) continue;
        vec3 L = normalize(uLightPosition[i] - vWorldPosition);
        vec3 R = reflect(-L, N);
        float diffuse = max(dot(N, L), 0.0);
        float specular = (diffuse > 0.0) ? pow(max(dot(R, V), 0.0), 24.0) : 0.0;
        color += uLightAmbient[i] * vColor;
        color += uLightDiffuse[i] * diffuse * vColor;
        color += uLightSpecular[i] * specular * 0.28;
    }
    FragColor = vec4(min(color, vec3(1.0)), 1.0);
}
)GLSL";

GLuint compileShader(GLenum type, const char* source)
{
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<size_t>(length > 0 ? length : 1), '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        glDeleteShader(shader);
        throw std::runtime_error("OpenGL shader compilation failed: " + log);
    }
    return shader;
}

GLuint createProgram()
{
    const GLuint vertex = compileShader(GL_VERTEX_SHADER, kVertexShader);
    GLuint fragment = 0;
    try {
        fragment = compileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    } catch (...) {
        glDeleteShader(vertex);
        throw;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<size_t>(length > 0 ? length : 1), '\0');
        glGetProgramInfoLog(program, length, nullptr, log.data());
        glDeleteProgram(program);
        throw std::runtime_error("OpenGL shader linking failed: " + log);
    }
    return program;
}

void setMat4(GLuint program, const char* name, const glm::mat4& matrix)
{
    const GLint location = glGetUniformLocation(program, name);
    if (location >= 0) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void setVec3(GLuint program, const std::string& name, const glm::vec3& vec)
{
    const GLint location = glGetUniformLocation(program, name.c_str());
    if (location >= 0) glUniform3fv(location, 1, glm::value_ptr(vec));
}

} // namespace

OpenglRenderSystem::~OpenglRenderSystem()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_program) glDeleteProgram(m_program);
}

void OpenglRenderSystem::init()
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to load OpenGL functions through glad");
    }
    std::cout << "OpenGL: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << '\n';

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_program = createProgram();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void OpenglRenderSystem::clearDisplay(float r, float g, float b) const
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderSystem::setViewport(double x, double y, double width, double height) const
{
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
               static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenglRenderSystem::renderTriangleSoup(const std::vector<Vertex>& vertices) const
{
    renderData(vertices, GL_TRIANGLES);
}

void OpenglRenderSystem::renderLines(const std::vector<Vertex>& vertices) const
{
    renderData(vertices, GL_LINES);
}

void OpenglRenderSystem::setupLight(uint32_t index, glm::vec3 position,
                                     glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    if (index >= m_lights.size()) throw std::out_of_range("Light index must be 0..7");
    auto& light = m_lights[index];
    light.position = position;
    light.ambient = ambient;
    light.diffuse = diffuse;
    light.specular = specular;
}

void OpenglRenderSystem::turnLight(uint32_t index, bool enable)
{
    if (index >= m_lights.size()) throw std::out_of_range("Light index must be 0..7");
    m_lights[index].enabled = enable;
}

void OpenglRenderSystem::renderData(const std::vector<Vertex>& vertices, unsigned int mode) const
{
    if (vertices.empty()) return;

    glUseProgram(m_program);
    setMat4(m_program, "uWorld", m_world);
    setMat4(m_program, "uView", m_view);
    setMat4(m_program, "uProjection", m_projection);
    setVec3(m_program, "uCameraPosition", m_cameraPosition);

    for (size_t i = 0; i < m_lights.size(); ++i) {
        const std::string suffix = "[" + std::to_string(i) + "]";
        const auto& light = m_lights[i];
        setVec3(m_program, "uLightPosition" + suffix, light.position);
        setVec3(m_program, "uLightAmbient" + suffix, light.ambient);
        setVec3(m_program, "uLightDiffuse" + suffix, light.diffuse);
        setVec3(m_program, "uLightSpecular" + suffix, light.specular);
        const GLint enabled = glGetUniformLocation(m_program, ("uLightEnabled" + suffix).c_str());
        if (enabled >= 0) glUniform1i(enabled, light.enabled ? 1 : 0);
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(static_cast<GLenum>(mode), 0, static_cast<GLsizei>(vertices.size()));
    glBindVertexArray(0);
}
