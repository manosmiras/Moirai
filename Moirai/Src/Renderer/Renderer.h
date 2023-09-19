#pragma once
#include "../Scene.h"

// TODO: Static?
class Renderer
{
public:
	Renderer(Scene* scene);
	void Render(float deltaTime);
private:
	Scene* scene;
};
