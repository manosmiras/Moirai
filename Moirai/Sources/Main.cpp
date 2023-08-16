#include <iostream>
#include <ostream>

#include "Renderer.h"
#include "Window.h"

int main()
{
    try
    {
        auto camera = std::make_shared<Camera>();
        auto window = std::make_shared<Window>(camera.get());
        
        Renderer renderer(window.get(), camera.get());
        
        float deltaTime;	// time between current frame and last frame
        float lastFrame = 0.0f;

        // Engine loop
        while (!window->ShouldClose())
        {
            const auto currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            renderer.Render(deltaTime);
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
