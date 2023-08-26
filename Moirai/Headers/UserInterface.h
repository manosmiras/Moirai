#pragma once
#include "Window.h"

class UserInterface
{
public:
    UserInterface(Window *window);
    ~UserInterface();
    void Setup(float deltaTime);
    void Render();
    glm::vec3 lightColor;
    glm::vec3 lightDirection;
};
