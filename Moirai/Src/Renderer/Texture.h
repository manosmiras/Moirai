#pragma once
#include <string>
#include <glad/glad.h>

enum TextureType
{
    Diffuse,
    Specular
};

class Texture
{
public:
    Texture(const std::string& filepath, TextureType textureType);
    void Bind();
    void Activate(int index);
    std::string GetSamplerName(int index);
private:
    GLuint id;
    TextureType textureType;
};
