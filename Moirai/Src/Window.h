#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>

#include "Camera.h"

class Window
{
public:
    GLFWwindow* window;
    Camera* camera;
    Window(Camera* camera);
    ~Window();
    void Update(float deltaTime);
    bool ShouldClose();
    float GetAspectRatio();
    void MouseCallback(double xposIn, double yposIn);
    void ScrollCallback(double xoffset, double yoffset);
    int width;
    int height;
    float lastX;
    float lastY;
    bool firstMouse = true;
};
