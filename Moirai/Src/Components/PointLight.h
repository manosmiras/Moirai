#pragma once
#include <glm/vec3.hpp>
#include "../Renderer/Shader.h"
struct PointLight
{
	Shader* shader;
	float constant;
	float linear;
	float quadratic;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};