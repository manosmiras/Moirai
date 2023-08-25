#pragma once
#include "Window.h"

class UserInterface
{
public:
    UserInterface(Window *window);
    ~UserInterface();
    void Setup();
    void Render();
    float lightColor[3];
};
