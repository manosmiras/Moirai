#pragma once
#include <map>
#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "UserInterface.h"
#include "Window.h"

class Renderer
{
public:
    Renderer(Window* window, Camera* camera, UserInterface* userInterface);
    ~Renderer();
    std::unique_ptr<Shader> phongShader;
    std::unique_ptr<Shader> lightSourceShader;
    std::map<GLuint, int> textures;
    unsigned int cubeVao, lightCubeVao, vbo;
    void Render(float deltaTime);
    Window* window;
    Camera* camera;
    UserInterface* userInterface;
    glm::vec3 lightPosition;
};
