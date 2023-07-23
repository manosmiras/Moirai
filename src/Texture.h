#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    static GLuint create_texture(const std::string& filepath, GLint image_format = GL_RGB);
};
