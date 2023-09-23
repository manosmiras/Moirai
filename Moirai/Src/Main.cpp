#include <iostream>
#include <ostream>
#include "entt/entt.hpp"
#include "Window.h"
#include "UserInterface.h"
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "Components/PointLight.h"
#include <random>
#include <stb_image.h>
#include "Renderer/Renderer.h"

int main()
{
	try
	{
		stbi_set_flip_vertically_on_load(true);
		auto camera = std::make_unique<Camera>();
		auto window = std::make_unique<Window>(camera.get());
		auto userInterface = std::make_unique<UserInterface>(window.get());
		
		auto shader = std::make_unique<Shader>("../Moirai/Shaders/TexturePhong.vert",
		                                       "../Moirai/Shaders/TexturePhong.frag");

		entt::registry registry;
		Scene scene{
			camera.get(),
			window.get(),
			userInterface->light.get(),
			registry
		};

		// Generate random position rotation and scale values
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution positionDist(-10.0f, 10.0f);
		std::uniform_real_distribution rotationDist(-90.0f, 90.0f);
		std::uniform_real_distribution scaleDist(1.0f, 2.0f);

		auto mesh = std::make_unique<Mesh>("../Moirai/Resources/backpack/backpack2.obj");
		auto plane = std::make_unique<Mesh>("../Moirai/Resources/meshes/plane.obj");

		auto diffuse = std::make_unique<Texture>("../Moirai/Resources/backpack/diffuse.jpg", Diffuse, glm::vec2(1.0f));
		auto specular = std::make_unique<Texture>("../Moirai/Resources/backpack/specular.jpg", Specular, glm::vec2(1.0f));

		auto map = std::make_unique<Texture>("../Moirai/Textures/uv_map.jpg", Diffuse, glm::vec2(10.0f));
		
		std::vector<Texture*> meshTextures;
		meshTextures.push_back(diffuse.get());
		meshTextures.push_back(specular.get());

		std::vector<Texture*> planeTextures;
		planeTextures.push_back(map.get());

		glm::vec3 meshPosition(1.0f);
		glm::vec3 rotation(0.0f);
		
		auto meshEntity = registry.create();
		registry.emplace<MeshRenderer>(meshEntity, shader.get(), mesh.get(), meshTextures);
		registry.emplace<Transform>(meshEntity, meshPosition, rotation, glm::vec3(1.0f));

		glm::vec3 planePosition(0.0f, -10.0f, 0.0f);
		
		auto planeEntity = registry.create();
		registry.emplace<MeshRenderer>(planeEntity, shader.get(), plane.get(), planeTextures);
		registry.emplace<Transform>(planeEntity, planePosition, rotation, glm::vec3(100.0f));

		glm::vec3 colors[] = {glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)};

		// Point lights
		for (size_t i = 0; i < 3; ++i)
		{
			auto entity = registry.create();
			glm::vec3 position(positionDist(gen), -10.0f, positionDist(gen));
			registry.emplace<Transform>(entity, position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
			registry.emplace<PointLight>(
				entity,
				shader.get(),
				1.0f, 0.09f, 0.032f,
				colors[i] * 0.5f,
				colors[i] * 0.8f,
				colors[i]
			);
		}
		
		Renderer renderer(&scene);

		float previousFrameTime = 0.0f;
		int frameCount = 0;
		// Engine loop
		while (!window->ShouldClose())
		{
			frameCount++;
			const auto currentTime = static_cast<float>(glfwGetTime());
			const float deltaTime = currentTime - previousFrameTime;
			previousFrameTime = currentTime;
			renderer.Render(deltaTime);
			userInterface->Render(deltaTime, renderer.frameBuffer.get());
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
