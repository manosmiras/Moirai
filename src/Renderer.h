#pragma once
#include <map>
#include <memory>

#include "Shader.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();
    std::unique_ptr<Shader> shader;
    std::map<GLuint, int> textures;
    unsigned int vao, vbo, ebo;
    void Render();
};
