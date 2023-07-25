#include <iostream>
#include <ostream>

#include "Renderer.h"
#include "Window.h"

int main()
{
    try
    {
        Window window;
        Renderer renderer;

        // Engine loop
        while (!window.ShouldClose())
        {
            window.PreRender();
            renderer.Render();
            window.PostRender();
        }
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return -1;
    }

    return 0;
}
