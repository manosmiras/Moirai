#pragma once
#include <map>
#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Window.h"

class Renderer
{
public:
    Renderer(Window* window, Camera* camera);
    ~Renderer();
    std::unique_ptr<Shader> shader;
    std::map<GLuint, int> textures;
    unsigned int vao, vbo, ebo;
    void Render(float deltaTime);
    Window* window;
    Camera* camera;
};
