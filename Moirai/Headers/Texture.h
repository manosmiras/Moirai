#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture(const std::string& filepath, GLuint &textureId);
};
