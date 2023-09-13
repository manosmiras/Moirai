#pragma once
#include <entt/entity/registry.hpp>

#include "Light.h"
#include "Window.h"

struct Scene
{
    Camera* camera;
    Window* window;
    Light* light;
    entt::registry& registry;
};
