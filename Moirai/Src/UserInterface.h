#pragma once
#include <memory>

#include "Light.h"
#include "Window.h"
#include "Renderer/FrameBuffer.h"

class UserInterface
{
public:
    UserInterface(Window *window);
    ~UserInterface();
    void Render(float deltaTime, FrameBuffer* frameBuffer);
    std::unique_ptr<Light> light;
};
