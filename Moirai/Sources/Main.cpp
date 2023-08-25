#include <iostream>
#include <ostream>

#include "Renderer.h"
#include "Window.h"
#include "UserInterface.h"

int main()
{
    try
    {
        auto camera = std::make_shared<Camera>();
        auto window = std::make_shared<Window>(camera.get());
        auto userInterface = std::make_shared<UserInterface>(window.get());
        
        Renderer renderer(window.get(), camera.get(), userInterface.get());

        float lastFrame = 0.0f;

        // Engine loop
        while (!window->ShouldClose())
        {
            const auto currentFrame = static_cast<float>(glfwGetTime());
            const float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            userInterface->Setup();
            renderer.Render(deltaTime);
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
