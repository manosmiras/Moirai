#pragma once

#include "../Renderer/Shader.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Texture.h"

struct MeshRenderer
{
	Shader* shader;
	Mesh* mesh;
	std::vector<Texture*> textures;
};