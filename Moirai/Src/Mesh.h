#pragma once
#include <vector>

#include "Shader.h"
#include "TextureComponent.h"
#include "Vertex.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureComponent> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureComponent> textures);
	void Setup();
	void Draw(Shader* shader);
private:
	unsigned int vao, vbo, ebo;
};
