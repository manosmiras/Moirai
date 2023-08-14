#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>
class Window
{
public:
    GLFWwindow* window;
    Window();
    ~Window();
    void Update();
    bool ShouldClose();
    float GetAspectRatio();
    int width;
    int height;
};
