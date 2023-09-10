#pragma once

#include "../Shader.h"

struct MeshRenderer
{
    Shader* shader;
    std::string modelPath;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int vao, vbo, ebo;
    std::vector<TextureComponent> textures;
};