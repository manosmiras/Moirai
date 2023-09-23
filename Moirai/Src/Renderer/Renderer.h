#pragma once
#include "../Scene.h"
#include "FrameBuffer.h"
// TODO: Static?
class Renderer
{
public:
	Renderer(Scene* scene);
	void Render(float deltaTime);
	std::unique_ptr<FrameBuffer> frameBuffer;
private:
	Scene* scene;
};
