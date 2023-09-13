#pragma once
#include <entt/entity/registry.hpp>

#include "../Scene.h"
#include "assimp/scene.h"

class MeshRendererSystem
{
public:
	void Setup(entt::registry& registry);
	void Update(entt::registry& registry);
	Scene* scene;
	std::string directory;
};
