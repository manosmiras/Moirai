#include <iostream>
#include <ostream>

#include "Renderer.h"
#include "Window.h"

int main()
{
    try
    {
        std::shared_ptr<Window> window = std::make_shared<Window>();
        
        Renderer renderer(window.get());

        // Engine loop
        while (!window->ShouldClose())
        {
            renderer.Render();
            window->Update();
        }
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return -1;
    }

    return 0;
}
