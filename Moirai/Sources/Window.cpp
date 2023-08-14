#include "Window.h"

#include <iostream>
#include <ostream>

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Window::Window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = 800;
    height = 600;
    
    window = glfwCreateWindow(width, height, "Moirai", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::exception("Failed to create GLFW window");
    }
	
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    gladLoadGL();
    glViewport(0, 0, 800, 600);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::Update()
{
    glfwSwapBuffers(window);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    glfwPollEvents();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

float Window::GetAspectRatio()
{
    return static_cast<float>(width) / static_cast<float>(height);
}
