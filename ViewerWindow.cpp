#include "ViewerWindow.h"

#include <stdexcept>
#include <utility>

ViewerWindow::ViewerWindow(const std::string& title, int width, int height)
{
    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_ptrHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_ptrHandle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create a GLFW window");
    }

    glfwMakeContextCurrent(m_ptrHandle);
    glfwSwapInterval(1); // VSync
    glfwSetWindowUserPointer(m_ptrHandle, this);
    glfwSetKeyCallback(m_ptrHandle, &ViewerWindow::keyCallback);
    glfwSetMouseButtonCallback(m_ptrHandle, &ViewerWindow::mouseButtonCallback);
    glfwSetCursorPosCallback(m_ptrHandle, &ViewerWindow::cursorCallback);
    glfwSetScrollCallback(m_ptrHandle, &ViewerWindow::scrollCallback);
}

ViewerWindow::~ViewerWindow()
{
    if (m_ptrHandle) {
        glfwDestroyWindow(m_ptrHandle);
    }
    glfwTerminate(); // This viewer owns its single GLFW window.
}

int ViewerWindow::getWidth() const
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_ptrHandle, &width, &height);
    return width;
}

int ViewerWindow::getHeight() const
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_ptrHandle, &width, &height);
    return height;
}

void ViewerWindow::keyCallback(GLFWwindow* window, int key, int, int action, int mods)
{
    auto* self = static_cast<ViewerWindow*>(glfwGetWindowUserPointer(window));
    if (self && self->m_keyCallback) {
        self->m_keyCallback(static_cast<KeyCode>(key), static_cast<Action>(action), mods);
    }
}

void ViewerWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto* self = static_cast<ViewerWindow*>(glfwGetWindowUserPointer(window));
    if (self && self->m_mouseButtonCallback) {
        self->m_mouseButtonCallback(static_cast<ButtonCode>(button), static_cast<Action>(action), mods);
    }
}

void ViewerWindow::cursorCallback(GLFWwindow* window, double x, double y)
{
    auto* self = static_cast<ViewerWindow*>(glfwGetWindowUserPointer(window));
    if (self && self->m_cursorCallback) {
        self->m_cursorCallback(x, y);
    }
}

void ViewerWindow::scrollCallback(GLFWwindow* window, double x, double y)
{
    auto* self = static_cast<ViewerWindow*>(glfwGetWindowUserPointer(window));
    if (self && self->m_scrollCallback) {
        self->m_scrollCallback(x, y);
    }
}
