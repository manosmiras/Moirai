#include <iostream>
#include <ostream>

#include "Systems/RendererSystem.h"
#include "Window.h"
#include "UserInterface.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "entt/entt.hpp"
#include <random>

int main()
{
    try
    {
        auto camera = std::make_unique<Camera>();
        auto window = std::make_unique<Window>(camera.get());
        auto userInterface = std::make_unique<UserInterface>(window.get());
        
        Scene scene;
        scene.camera = camera.get();
        scene.window = window.get();
        scene.light = &userInterface->light;

        auto shader = std::make_unique<Shader>("../Moirai/Shaders/TexturePhong.vert", "../Moirai/Shaders/TexturePhong.frag");

        entt::registry registry;

        // Generate random values for position within the range of -10 to 10
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
        
        for (size_t i = 0; i < 10; ++i)
        {
            auto entity = registry.create();
            registry.emplace<Renderer>(entity, shader.get());
            glm::vec3 position(dist(gen), dist(gen), dist(gen));
            registry.emplace<Transform>(entity, position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
        }
        RendererSystem rendererSystem;
        rendererSystem.scene = &scene;
        
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
