#include "Shader.h"

#include <fstream>
#include <ostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    // Read shader contents
    const std::string vertexContent = ReadFileContents(vertexPath);
    const char* vertexCode = vertexContent.c_str();
    const std::string fragmentContent = ReadFileContents(fragmentPath);
    const char* fragmentCode = fragmentContent.c_str();

    // Compile vertex shader
    const auto vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    CheckCompilationErrors(vertex, "VERTEX");
    // Compile fragment Shader
    const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    CheckCompilationErrors(fragment, "FRAGMENT");
    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    CheckCompilationErrors(id, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::SetBool(const std::string &name, const bool &value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string &name, const int &value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, const float &value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::CheckCompilationErrors(const GLuint shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            throw std::exception(infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            throw std::exception(infoLog);
        }
    }
}

std::string Shader::ReadFileContents(const std::string& filepath)
{
    std::ifstream file;
    file.open(filepath);

    std::string fileContents;
    std::stringstream stream;
    while (getline(file, fileContents))
    {
        // Remove BOM from UTF-8 files
        if (fileContents.compare(0, 3, "\xEF\xBB\xBF") == 0) 
        {
            fileContents = fileContents.substr(3, fileContents.size());
        }
        stream << fileContents << "\n";
    }
    return stream.str();
}
