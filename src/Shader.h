#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
public:
    GLuint id;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();
    void DeleteProgram();
    void SetBool(const std::string &name, const bool value) const;
    void SetInt(const std::string &name, const int value) const;
    void SetFloat(const std::string &name, const float value) const;
private:
    void CheckCompilationErrors(const GLuint shader, const std::string& type);
    std::string ReadFileContents(const std::string& filepath);
};
