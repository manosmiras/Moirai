#pragma once
#include <vector>
#include "Vertex.h"
#include "assimp/scene.h"
#include <glad/glad.h>

class Mesh
{
public:
	Mesh(std::string path);
	void Draw();
private:
	void ProcessMesh(aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	GLuint vao, vbo, ebo;
};
