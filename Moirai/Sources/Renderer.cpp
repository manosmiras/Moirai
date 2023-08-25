#include "Renderer.h"
#include <glad/glad.h>
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLFW/glfw3.h"

Renderer::Renderer(Window* window, Camera* camera, UserInterface* userInterface)
{
    lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
    this->window = window;
    this->camera = camera;
    this->userInterface = userInterface;
    phongShader = std::make_unique<Shader>("../Moirai/Shaders/TexturePhong.vert", "../Moirai/Shaders/TexturePhong.frag");
    lightSourceShader = std::make_unique<Shader>("../Moirai/Shaders/LightSource.vert", "../Moirai/Shaders/LightSource.frag");
    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVao);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightCubeVao);
    glBindVertexArray(lightCubeVao);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLuint diffuse, specular;
    
    Texture("../Moirai/Textures/container.png", diffuse);
    Texture("../Moirai/Textures/container_specular.png", specular);
    
    textures.insert(std::pair<GLuint, int>(diffuse, GL_TEXTURE0));
    textures.insert(std::pair<GLuint, int>(specular, GL_TEXTURE1));
    
    phongShader->Use();
    phongShader->SetInt("material.diffuse", 0);
    phongShader->SetInt("material.specular", 1);
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &cubeVao);
    glDeleteVertexArrays(1, &lightCubeVao);
    glDeleteBuffers(1, &vbo);
}

void Renderer::Render(float deltaTime)
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 model(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), window->GetAspectRatio(), 0.1f, 100.0f);
    
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0, 1.0f, 1.0f));
    
    auto view = camera->GetViewMatrix();

    phongShader->Use();
    // positions
    phongShader->SetVec3("light.position", lightPosition);
    phongShader->SetVec3("viewPosition", camera->Position);
    
    phongShader->SetVec3("material.specular",glm::vec3(0.5f, 0.5f, 0.5f));
    phongShader->SetFloat("material.shininess", 64.0f);

    const auto lightColor = glm::vec3(userInterface->lightColor[0],  userInterface->lightColor[1],  userInterface->lightColor[2]);

    auto ambient = lightColor * 0.2f;
    auto diffuse = lightColor * 0.5f;
    auto specular = lightColor;

    // light values
    phongShader->SetVec3("light.ambient",  ambient);
    phongShader->SetVec3("light.diffuse",  diffuse);
    phongShader->SetVec3("light.specular", specular); 

    // mvp
    phongShader->SetMat4("model", model);
    phongShader->SetMat4("view", view);
    phongShader->SetMat4("projection", projection);

    // Bind textures
    for (const auto &texture : textures)
    {
        glActiveTexture(texture.second);
        glBindTexture(GL_TEXTURE_2D, texture.first);
    }

    // render textured cube
    glBindVertexArray(cubeVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    
    model = glm::translate(model, lightPosition);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

    lightSourceShader->Use();
    lightSourceShader->SetVec3("lightColor", lightColor);
    lightSourceShader->SetMat4("model", model);
    lightSourceShader->SetMat4("view", view);
    lightSourceShader->SetMat4("projection", projection);

    // render light cube
    glBindVertexArray(lightCubeVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
