#pragma once
#include <vector>
#include "Vertex.h"
#include "assimp/scene.h"

class Mesh
{
public:
	Mesh(std::string path);
	void Draw();
private:
	void ProcessMesh(aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int vao, vbo, ebo;
};
