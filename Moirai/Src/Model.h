#pragma once
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "assimp/scene.h"

class Model
{
public:
	std::vector<TextureComponent> texturesLoaded;
	Model(char* path);
	void Draw(Shader* shader);
	std::vector<TextureComponent> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
