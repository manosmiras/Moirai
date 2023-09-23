#include "UserInterface.h"

#include <memory>
#include <string>
#include <glm/gtc/type_ptr.inl>

#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Renderer/FrameBuffer.h"

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
    ImGui::StyleColorsDark();
    
    light = std::make_unique<Light>();
    light->lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light->lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
}

UserInterface::~UserInterface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::Render(float deltaTime, FrameBuffer* frameBuffer)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::NewFrame();
    
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
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
    ImGui::ColorEdit3("Light Color",  glm::value_ptr(light->lightColor));
    ImGui::SliderFloat3("Light Direction", glm::value_ptr(light->lightDirection), -1.0f, 1.0f);
    
    ImGui::End();

    ImGui::Begin("Scene");

    const float width = ImGui::GetContentRegionAvail().x;
    const float height = ImGui::GetContentRegionAvail().y;

    frameBuffer->Rescale(width, height);
    
    glViewport(0, 0, width, height);

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage(
        (void*)frameBuffer->textureId,
        ImVec2(pos.x, pos.y), 
            ImVec2(pos.x + width, pos.y + height), 
            ImVec2(0, 1), 
            ImVec2(1, 0)
    );

    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
