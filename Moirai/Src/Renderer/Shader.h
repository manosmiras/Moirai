#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/fwd.hpp>

class Shader
{
public:
    GLuint id;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    ~Shader();
    void Use();
    void SetBool(const std::string &name, const bool &value) const;
    void SetInt(const std::string &name, const int &value) const;
    void SetFloat(const std::string &name, const float &value) const;
    void SetMat4(const std::string &name, const glm::mat4 &value) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
private:
    void CheckCompilationErrors(const GLuint shader, const std::string& type);
    std::string ReadFileContents(const std::string &filepath);
};
