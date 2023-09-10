#pragma once
#include <entt/entity/registry.hpp>

#include "../Scene.h"
#include "assimp/material.h"
#include "../Components/TextureComponent.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "../Components/Vertex.h"

class MeshRendererSystem
{
public:
    void Setup(entt::registry &registry);
    void Update(entt::registry &registry);
    std::vector<TextureComponent> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<TextureComponent> &textures);
    Scene* scene;
    std::string directory;
    std::vector<TextureComponent> texturesLoaded;
};
