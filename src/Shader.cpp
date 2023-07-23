#include "Shader.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path)
{
    // Read shader contents
    const std::string vertex_content = read_file_contents(vertex_path);
    const char* vertex_code = vertex_content.c_str();
    const std::string fragment_content = read_file_contents(fragment_path);
    const char* fragment_code = fragment_content.c_str();
    // Compile vertex shader
    const auto vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, nullptr);
    glCompileShader(vertex);
    check_compilation_errors(vertex, "VERTEX");
    // Compile fragment Shader
    const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, nullptr);
    glCompileShader(fragment);
    check_compilation_errors(fragment, "FRAGMENT");
    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compilation_errors(id, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::delete_program()
{
    glDeleteProgram(id);
}
    
void Shader::set_bool(const std::string &name, const bool value) const
{         
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value)); 
}
    
void Shader::set_int(const std::string &name, const int value) const
{ 
    glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
}
    
void Shader::set_float(const std::string &name, const float value) const
{ 
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
}

void Shader::check_compilation_errors(const GLuint shader, const std::string& type)
{
    int success;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << std::endl;
        }
    }
}

std::string Shader::read_file_contents(const std::string& filepath)
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