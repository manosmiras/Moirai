#pragma once
#include <entt/entity/registry.hpp>

#include "../Scene.h"

class RendererSystem
{
public:
    void Setup(entt::registry &registry);
    void Update(entt::registry &registry);
    Scene* scene;
};
