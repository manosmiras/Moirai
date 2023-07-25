#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>
class Window
{
public:
    GLFWwindow* window;
    Window();
    ~Window();
    void PostRender();
    void PreRender();
    bool ShouldClose();
};
