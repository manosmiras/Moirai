#include "Shader.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

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

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::DeleteProgram()
{
    glDeleteProgram(id);
}
    
void Shader::SetBool(const std::string &name, const bool value) const
{         
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value)); 
}
    
void Shader::SetInt(const std::string &name, const int value) const
{ 
    glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
}
    
void Shader::SetFloat(const std::string &name, const float value) const
{ 
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
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
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}

std::string Shader::ReadFileContents(const std::string& filepath)
{
    std::string contents;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        file.open(filepath);
        std::stringstream stream;
        // read file's buffer contents into streams
        stream << file.rdbuf();
        // close file handlers
        file.close();
        // convert stream into string
        contents = stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
        
    return contents;
}