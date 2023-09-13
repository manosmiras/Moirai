#include "Renderer.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"
#include <entt/entity/registry.hpp>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Renderer::Renderer(Scene* scene)
{
	glEnable(GL_DEPTH_TEST);
	this->scene = scene;
}

void Renderer::Render()
{
	entt::registry& registry = scene->registry;

	auto view = registry.view<MeshRenderer, Transform>();

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto camera = scene->camera;
	auto window = scene->window;
	auto lightDirection = scene->light->lightDirection;
	auto lightColor = scene->light->lightColor;

	auto frontEntity = view.front();
	auto frontRenderer = registry.get<MeshRenderer>(frontEntity);
	auto shader = frontRenderer.shader;

	shader->Use();
	shader->SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader->SetFloat("material.shininess", 64.0f);
	auto ambient = lightColor * 0.2f;
	auto diffuse = lightColor * 0.5f;
	auto specular = lightColor;

	// light values
	shader->SetVec3("directionalLight.direction", lightDirection);
	shader->SetVec3("directionalLight.ambient", ambient);
	shader->SetVec3("directionalLight.diffuse", diffuse);
	shader->SetVec3("directionalLight.specular", specular);

	// camera
	shader->SetVec3("viewPosition", camera->Position);

	// view and projection matrices
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), window->GetAspectRatio(), 0.1f,
	                                              100.0f);
	auto viewMatrix = camera->GetViewMatrix();
	shader->SetMat4("view", viewMatrix);
	shader->SetMat4("projection", projectionMatrix);

	view.each([shader](MeshRenderer& renderer, Transform& transform)
	{
		auto model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);
		glm::quat rotation;
		rotation = glm::quat(transform.rotation);
		model *= glm::toMat4(rotation);
		model = glm::scale(model, transform.scale);
		shader->SetMat4("model", model);
		renderer.mesh->Draw();
		for(int i = 0; i < renderer.textures.size(); ++i)
		{
			auto texture = renderer.textures[i];
			texture->Activate(i);
			renderer.shader->SetInt(texture->GetSamplerName(i), i);
			texture->Bind();
		}
		/*unsigned int diffuseIndex = 1;
		unsigned int specularIndex = 1;
		for(unsigned int i = 0; i < renderer.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = renderer.textures[i].type;
			if(name == "diffuse")
				number = std::to_string(diffuseIndex++);
			else if(name == "specular")
				number = std::to_string(specularIndex++);

			shader->SetInt(("material" + number + "." + name).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, renderer.textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);*/
	});

	/*auto lightView = registry.view<PointLight, Transform>();
	auto lightIndex = 0;
	for(auto entity: lightView) {
	    auto &pointLight = lightView.get<PointLight>(entity);
	    auto &transform = lightView.get<Transform>(entity);
	    std::string pointLightUniform = "pointLights[" + std::to_string(lightIndex) + "]";
	    shader->SetVec3(pointLightUniform + ".position", transform.position);
	    shader->SetVec3(pointLightUniform + ".ambient", pointLight.ambient);
	    shader->SetVec3(pointLightUniform + ".diffuse", pointLight.diffuse);
	    shader->SetVec3(pointLightUniform + ".specular", pointLight.specular);
	    shader->SetFloat(pointLightUniform + ".constant", pointLight.constant);
	    shader->SetFloat(pointLightUniform + ".linear", pointLight.linear);
	    shader->SetFloat(pointLightUniform + ".quadratic", pointLight.quadratic);
	    lightIndex++;
	}*/
}
