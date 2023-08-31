#pragma once
#include "Light.h"
#include "Window.h"

class UserInterface
{
public:
    UserInterface(Window *window);
    ~UserInterface();
    void Setup(float deltaTime);
    void Render();
    Light light;
};
