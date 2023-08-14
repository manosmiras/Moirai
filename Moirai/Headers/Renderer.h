#pragma once
#include <map>
#include <memory>

#include "Shader.h"
#include "Window.h"

class Renderer
{
public:
    Renderer(Window* window);
    ~Renderer();
    std::unique_ptr<Shader> shader;
    std::map<GLuint, int> textures;
    unsigned int vao, vbo, ebo;
    void Render();
    Window* window;
};
