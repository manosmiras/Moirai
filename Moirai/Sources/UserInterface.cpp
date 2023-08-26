#include "UserInterface.h"

#include <string>
#include <glm/gtc/type_ptr.inl>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

UserInterface::UserInterface(Window* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
}

UserInterface::~UserInterface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::Setup(float deltaTime)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Properties");
    ImGui::LabelText("Frame Time", std::to_string(deltaTime).c_str());
    float fps = 1.0f / deltaTime;
    ImGui::LabelText("FPS", std::to_string(fps).c_str());
    
    ImGui::ColorEdit3("Light Color",  glm::value_ptr(lightColor));
    ImGui::DragFloat3("Light Direction", glm::value_ptr(lightDirection));
    ImGui::End();
}

void UserInterface::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
