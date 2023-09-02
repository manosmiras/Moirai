#include "RendererSystem.h"

#include <entt/entity/registry.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../Texture.h"
#include "../Components/PointLight.h"
#include "../Components/Renderer.h"
#include "../Components/Transform.h"

void RendererSystem::Setup(entt::registry &registry)
{
    auto view = registry.view<Renderer>();
    view.each([](Renderer &renderer)
    {
        glGenVertexArrays(1, &renderer.vao);
        glGenBuffers(1, &renderer.vbo);

        glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
        glBufferData(GL_ARRAY_BUFFER, renderer.vertices->size() * sizeof(float), renderer.vertices->data(), GL_STATIC_DRAW);

        glBindVertexArray(renderer.vao);
 
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        GLuint diffuse, specular;
    
        Texture("../Moirai/Textures/container.png", diffuse);
        Texture("../Moirai/Textures/container_specular.png", specular);
    
        renderer.textures.insert(std::pair<GLuint, int>(diffuse, GL_TEXTURE0));
        renderer.textures.insert(std::pair<GLuint, int>(specular, GL_TEXTURE1));
    
        renderer.shader->Use();
        renderer.shader->SetInt("material.diffuse", 0);
        renderer.shader->SetInt("material.specular", 1);
    });
    
    glEnable(GL_DEPTH_TEST);
}

void RendererSystem::Update(entt::registry &registry)
{
    auto view = registry.view<Renderer, Transform>();
    
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto camera = scene->camera;
    auto window = scene->window;
    auto lightDirection = scene->light->lightDirection;
    auto lightColor = scene->light->lightColor;
    
    auto frontEntity = view.front();
    auto frontRenderer = registry.get<Renderer>(frontEntity);
    auto shader = frontRenderer.shader;
    
    shader->Use();
    shader->SetVec3("material.specular",glm::vec3(0.5f, 0.5f, 0.5f));
    shader->SetFloat("material.shininess", 64.0f);
    auto ambient = lightColor * 0.2f;
    auto diffuse = lightColor * 0.5f;
    auto specular = lightColor;

    // light values
    shader->SetVec3("directionalLight.direction", lightDirection);
    shader->SetVec3("directionalLight.ambient",  ambient);
    shader->SetVec3("directionalLight.diffuse",  diffuse);
    shader->SetVec3("directionalLight.specular", specular);

    // camera
    shader->SetVec3("viewPosition", camera->Position);
        
    // view and projection matrices
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), window->GetAspectRatio(), 0.1f, 100.0f);
    auto viewMatrix = camera->GetViewMatrix();
    shader->SetMat4("view", viewMatrix);
    shader->SetMat4("projection", projectionMatrix);

    view.each([shader](Renderer &renderer, Transform &transform)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0, 1.0f, 1.0f));
        model = glm::scale(model, transform.scale);
        shader->SetMat4("model", model);
        // Bind textures
        for (const auto &texture : renderer.textures)
        {
            glActiveTexture(texture.second);
            glBindTexture(GL_TEXTURE_2D, texture.first);
        }
        glBindVertexArray(renderer.vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    });
    
    auto lightView = registry.view<PointLight, Transform>();
    auto lightIndex = 0;
    for(auto entity: lightView) {
        auto &pointLight = lightView.get<PointLight>(entity);
        auto &transform = lightView.get<Transform>(entity);
        std::string pointLightUniform = "pointLights[" + std::to_string(lightIndex) + "]";
        shader->SetVec3(pointLightUniform + ".position", transform.position);
        shader->SetVec3(pointLightUniform + ".ambient", pointLight.ambient);
        shader->SetVec3(pointLightUniform + ".diffuse", pointLight.diffuse);
        shader->SetVec3(pointLightUniform + ".specular", pointLight.specular);
        shader->SetFloat(pointLightUniform + ".constant", pointLight.constant);
        shader->SetFloat(pointLightUniform + ".linear", pointLight.linear);
        shader->SetFloat(pointLightUniform + ".quadratic", pointLight.quadratic);
        lightIndex++;
    }
}