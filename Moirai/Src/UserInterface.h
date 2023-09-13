#pragma once
#include <memory>

#include "Light.h"
#include "Window.h"

class UserInterface
{
public:
    UserInterface(Window *window);
    ~UserInterface();
    void Setup(float deltaTime);
    void Render();
    std::unique_ptr<Light> light;
};
