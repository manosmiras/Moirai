#pragma once
#include "../Scene.h"

// TODO: Static?
class Renderer
{
public:
	Renderer(Scene* scene);
	void Render();
private:
	Scene* scene;
};
