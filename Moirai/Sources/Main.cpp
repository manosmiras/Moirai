#include <iostream>
#include <ostream>

#include "Renderer.h"
#include "Window.h"
#include "UserInterface.h"

int main()
{
    try
    {
        auto camera = std::make_unique<Camera>();
        auto window = std::make_unique<Window>(camera.get());
        auto userInterface = std::make_unique<UserInterface>(window.get());
        
        Renderer renderer(window.get(), camera.get(), userInterface.get());

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
