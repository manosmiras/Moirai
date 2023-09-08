#include <iostream>
#include <ostream>

#include "Systems/RendererSystem.h"
#include "Window.h"
#include "UserInterface.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "entt/entt.hpp"
#include <random>
#include <stb_image.h>

#include "VertexUtils.h"
#include "Components/PointLight.h"

int main()
{
    try
    {
        stbi_set_flip_vertically_on_load(true);
        auto camera = std::make_unique<Camera>();
        auto window = std::make_unique<Window>(camera.get());
        auto userInterface = std::make_unique<UserInterface>(window.get());
        
        Scene scene;
        scene.camera = camera.get();
        scene.window = window.get();
        scene.light = &userInterface->light;

        auto shader = std::make_unique<Shader>("../Moirai/Shaders/TexturePhong.vert", "../Moirai/Shaders/TexturePhong.frag");

        entt::registry registry;

        // Generate random position rotation and scale values
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution positionDist(-10.0f, 10.0f);
        std::uniform_real_distribution rotationDist(-90.0f, 90.0f);
        std::uniform_real_distribution scaleDist(1.0f, 2.0f);

        auto model = Model("../Moirai/Resources/backpack/backpack.obj");

        // Meshes
        for (size_t i = 0; i < 1; ++i)
        {
            auto entity = registry.create();
            registry.emplace<Renderer>(entity, shader.get(), &model);
            registry.emplace<Transform>(entity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
        }

        /*// Cubes
        for (size_t i = 0; i < 10; ++i)
        {
            auto entity = registry.create();
            registry.emplace<Renderer>(entity, shader.get(), &VertexUtils::cube);
            glm::vec3 position(positionDist(gen), positionDist(gen), positionDist(gen));
            glm::vec3 rotation(rotationDist(gen), rotationDist(gen), rotationDist(gen));
            glm::vec3 scale(scaleDist(gen), scaleDist(gen), scaleDist(gen));
            registry.emplace<Transform>(entity, position, rotation, scale);
        }

        // Planes
        for (size_t i = 0; i < 1; ++i)
        {
            auto entity = registry.create();
            registry.emplace<Renderer>(entity, shader.get(), &VertexUtils::plane);
            registry.emplace<Transform>(entity, glm::vec3(0.0f, -20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f));
        }
        
        // Point lights
        for (size_t i = 0; i < 4; ++i)
        {
            auto entity = registry.create();
            glm::vec3 position(positionDist(gen), positionDist(gen), positionDist(gen));
            registry.emplace<Transform>(entity, position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
            registry.emplace<PointLight>(
                entity,
                shader.get(),
                1.0f, 0.09f, 0.032f,
                glm::vec3(0.05f, 0.05f, 0.05f),
                glm::vec3(0.8f, 0.8f, 0.8f),
                glm::vec3(1.0f, 1.0f, 1.0f)
            );
        }*/
        
        RendererSystem rendererSystem{&scene};
        
        rendererSystem.Setup(registry);
        
        float previousFrameTime = 0.0f;
        int frameCount = 0;
        // Engine loop
        while (!window->ShouldClose())
        {
            frameCount++;
            const auto currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime = currentTime - previousFrameTime;
            previousFrameTime = currentTime;
            
            userInterface->Setup(deltaTime);
            rendererSystem.Update(registry);
            userInterface->Render();
            window->Update(deltaTime);
        }
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return -1;
    }
    return 0;
}
