#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <utility>

enum class Action : int {
    Release = GLFW_RELEASE,
    Press = GLFW_PRESS,
    Repeat = GLFW_REPEAT
};

enum class KeyCode : int {
    Escape = GLFW_KEY_ESCAPE,
    Space = GLFW_KEY_SPACE,
    Up = GLFW_KEY_UP,
    Down = GLFW_KEY_DOWN,
    Left = GLFW_KEY_LEFT,
    Right = GLFW_KEY_RIGHT,
    C = GLFW_KEY_C,
    P = GLFW_KEY_P,
    R = GLFW_KEY_R,
    Num1 = GLFW_KEY_1,
    Num2 = GLFW_KEY_2
};

enum class ButtonCode : int {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE
};

using Modifier = int; // GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | ...

class ViewerWindow {
public:
    using KeyCallback = std::function<void(KeyCode, Action, Modifier)>;
    using MouseButtonCallback = std::function<void(ButtonCode, Action, Modifier)>;
    using CursorCallback = std::function<void(double, double)>;
    using ScrollCallback = std::function<void(double, double)>;

    ViewerWindow(const std::string& title, int width, int height);
    ~ViewerWindow();
    ViewerWindow(const ViewerWindow&) = delete;
    ViewerWindow& operator=(const ViewerWindow&) = delete;

    GLFWwindow* getGLFWHandle() const { return m_ptrHandle; }
    int getWidth() const;
    int getHeight() const;
    void setKeyCallback(KeyCallback callback) { m_keyCallback = std::move(callback); }
    void setMouseButtonCallback(MouseButtonCallback callback) { m_mouseButtonCallback = std::move(callback); }
    void setCursorCallback(CursorCallback callback) { m_cursorCallback = std::move(callback); }
    void setScrollCallback(ScrollCallback callback) { m_scrollCallback = std::move(callback); }

private:
    static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow* window, double x, double y);
    static void scrollCallback(GLFWwindow* window, double x, double y);

    GLFWwindow* m_ptrHandle = nullptr;
    KeyCallback m_keyCallback;
    MouseButtonCallback m_mouseButtonCallback;
    CursorCallback m_cursorCallback;
    ScrollCallback m_scrollCallback;
};
