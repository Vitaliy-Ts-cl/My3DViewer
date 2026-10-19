#include <glad/glad.h> // Must be included before GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

int main()
{
    const glm::vec3 vector(1.0f, 0.0f, 0.0f);
    std::cout << "Vector: (" << vector.x << ", " << vector.y
              << ", " << vector.z << ")\n";

    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "ERROR: GLFW initialization failed.\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(800, 600, "My3DViewer - Lab 7", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "ERROR: Cannot create an OpenGL window.\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        std::cerr << "ERROR: Glad initialization failed.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    std::cout << "OpenGL: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << '\n';
    std::cout << "LAB 7 SUCCESS: GLM, GLFW and Glad initialized.\n";
    std::cout << "Close the window to exit.\n";

    glfwSwapInterval(1);

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

        glViewport(0, 0, framebufferWidth, framebufferHeight);
        glClearColor(0.10f, 0.14f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
