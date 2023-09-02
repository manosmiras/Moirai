#pragma once

#include <map>

#include "../Shader.h"

struct Renderer
{
    Shader* shader;
    std::vector<float>* vertices;
    unsigned int vao;
    unsigned int vbo;
    std::map<GLuint, int> textures;
};
