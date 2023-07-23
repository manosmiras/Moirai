#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
public:
    GLuint id;
    Shader(const GLchar* vertex_path, const GLchar* fragment_path);
    void use();
    void delete_program();
    void set_bool(const std::string &name, const bool value) const;
    void set_int(const std::string &name, const int value) const;
    void set_float(const std::string &name, const float value) const;
private:
    void check_compilation_errors(const GLuint shader, const std::string& type);
    std::string read_file_contents(const std::string& filepath);
};
