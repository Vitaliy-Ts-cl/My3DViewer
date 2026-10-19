#include "GeometryPrimitives.h"
#include "OpenglRenderSystem.h"
#include "ViewerWindow.h"

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace {

enum class Shape { Pyramid, Cube };

void renderScene(OpenglRenderSystem& renderer, Shape shape)
{
    static const auto cube = GeometryPrimitives::createCubeVertices();
    static const auto pyramid = GeometryPrimitives::createPyramidVertices();
    renderer.renderTriangleSoup(shape == Shape::Pyramid ? pyramid : cube);
}

} // namespace

int main()
{
    try {
        constexpr int width = 800;
        constexpr int height = 600;

        // The window must outlive the renderer: OpenGL resources need its context.
        ViewerWindow window("My3DViewer - Lab 8 (Pyramid / Cube)", width, height);
        OpenglRenderSystem renderer;
        renderer.init();
        renderer.setupLight(0, {2.5f, 4.0f, 3.0f}, {0.16f, 0.16f, 0.16f},
                            {0.86f, 0.86f, 0.86f}, {0.50f, 0.50f, 0.50f});
        renderer.turnLight(0, true);

        Shape currentShape = Shape::Pyramid; // Final assignment: show the pyramid first.
        glm::vec3 objectOffset(0.0f);
        float yaw = 0.50f;
        float pitch = 0.35f;
        float distance = 3.4f;
        bool dragging = false;
        double lastX = 0.0;
        double lastY = 0.0;

        window.setKeyCallback([&](KeyCode key, Action action, Modifier) {
            if (action != Action::Press && action != Action::Repeat) return;
            constexpr float step = 0.12f;
            switch (key) {
            case KeyCode::Escape:
                glfwSetWindowShouldClose(window.getGLFWHandle(), GLFW_TRUE);
                break;
            case KeyCode::Up:    objectOffset.y += step; break;
            case KeyCode::Down:  objectOffset.y -= step; break;
            case KeyCode::Left:  objectOffset.x -= step; break;
            case KeyCode::Right: objectOffset.x += step; break;
            case KeyCode::C:
            case KeyCode::Num2:  currentShape = Shape::Cube; break;
            case KeyCode::P:
            case KeyCode::Num1:  currentShape = Shape::Pyramid; break;
            case KeyCode::R:
                objectOffset = glm::vec3(0.0f);
                yaw = 0.50f;
                pitch = 0.35f;
                distance = 3.4f;
                break;
            default: break;
            }
        });

        window.setMouseButtonCallback([&](ButtonCode button, Action action, Modifier) {
            if (button != ButtonCode::Left) return;
            dragging = (action == Action::Press);
            if (dragging) glfwGetCursorPos(window.getGLFWHandle(), &lastX, &lastY);
        });

        window.setCursorCallback([&](double x, double y) {
            if (dragging) {
                yaw += static_cast<float>(x - lastX) * 0.007f;
                pitch = std::clamp(pitch - static_cast<float>(y - lastY) * 0.007f,
                                   -1.4f, 1.4f);
            }
            lastX = x;
            lastY = y;
        });

        window.setScrollCallback([&](double, double y) {
            distance = std::clamp(distance - static_cast<float>(y) * 0.20f,
                                  1.6f, 12.0f);
        });

        std::cout << "LAB 8: OpenGL renderer initialized successfully.\n"
                  << "Default: pyramid (4 triangular sides + square base).\n"
                  << "P / 1: pyramid, C / 2: cube, arrows: move, "
                     "left drag: orbit, scroll: zoom, R: reset, Esc: quit.\n";

        while (!glfwWindowShouldClose(window.getGLFWHandle())) {
            glfwPollEvents();
            const int framebufferWidth = window.getWidth();
            const int framebufferHeight = window.getHeight();
            if (framebufferWidth <= 0 || framebufferHeight <= 0) {
                glfwWaitEvents();
                continue;
            }

            const glm::vec3 eye(
                distance * std::sin(yaw) * std::cos(pitch),
                distance * std::sin(pitch),
                distance * std::cos(yaw) * std::cos(pitch));

            renderer.setCameraPosition(eye);
            renderer.setWorldMatrix(glm::translate(glm::mat4(1.0f), objectOffset));
            renderer.setViewMatrix(glm::lookAt(eye, glm::vec3(0.0f),
                                                glm::vec3(0.0f, 1.0f, 0.0f)));
            renderer.setProjectionMatrix(glm::perspective(
                glm::radians(60.0f),
                static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight),
                0.1f, 100.0f));

            renderer.setViewport(0, 0, framebufferWidth, framebufferHeight);
            renderer.clearDisplay(0.12f, 0.15f, 0.20f);
            renderScene(renderer, currentShape);
            glfwSwapBuffers(window.getGLFWHandle());
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}
