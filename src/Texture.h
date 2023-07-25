#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    static GLuint CreateTexture(const std::string& filepath, GLint imageFormat = GL_RGB);
};
