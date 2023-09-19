#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/vec2.hpp>

enum TextureType
{
    Diffuse,
    Specular
};

class Texture
{
public:
    Texture(const std::string& filepath, TextureType textureType, glm::vec2 uvScale);
    void Bind();
    void Activate(int index);
    std::string GetSamplerName(int index);
    glm::vec2 uvScale;
private:
    GLuint id;
    TextureType textureType;
};
