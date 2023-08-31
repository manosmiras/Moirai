#pragma once
#include "Light.h"
#include "Window.h"

struct Scene
{
    Camera* camera;
    Window* window;
    Light* light;
};
