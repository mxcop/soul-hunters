#include "shader.h"

#include <iostream>

Shader& Shader::use()
{
    glUseProgram(this->id);
    return *this;
}

bool Shader::compile(const char* vert_src, const char* frag_src, const char* geo_src)
{
    // Confirm that both the vertex and fragment shader are not null:
    if (vert_src == nullptr || frag_src == nullptr) return false;

    unsigned int vert_shader, frag_shader, geo_shader;

    // Compile vertex shader:
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_src, NULL);
    glCompileShader(vert_shader);
    if (!log_err(vert_shader, "VERTEX")) return false;

    // Compile fragment shader:
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_src, NULL);
    glCompileShader(frag_shader);
    if (!log_err(frag_shader, "FRAGMENT")) return false;

    if (geo_src != nullptr) {
        // Compile geometry shader:
        geo_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geo_shader, 1, &geo_src, NULL);
        glCompileShader(geo_shader);
        if (!log_err(geo_shader, "GEOMETRY")) return false;
    }

    // Create the shader program.
    this->id = glCreateProgram();

    // Attach the compiled shaders:
    glAttachShader(this->id, vert_shader);
    glAttachShader(this->id, frag_shader);
    if (geo_src != nullptr) {
        glAttachShader(this->id, geo_shader);
    }

    // Link the shader program.
    glLinkProgram(this->id);
    if (!log_err(this->id, "PROGRAM")) return false;

    // Finally, delete the shaders once linked.
    // Now they're stored on the GPU.
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    if (geo_src != nullptr) {
        glDeleteShader(geo_shader);
    }

    return true;
}

bool Shader::log_err(unsigned int shader_id, std::string type) const
{
    int success;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_id, 1024, NULL, info_log);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << info_log << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader_id, 1024, NULL, info_log);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << info_log << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    return success;
}

void Shader::set_float(const char* name, float value, bool useShader)
{
    if (useShader) this->use();
    glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::set_int(const char* name, int value, bool useShader)
{
    if (useShader) this->use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::set_vec2f(const char* name, float x, float y, bool useShader)
{
    if (useShader) this->use();
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}

void Shader::set_vec2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader) this->use();
    glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}

void Shader::set_vec3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader) this->use();
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void Shader::set_vec3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader) this->use();
    glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}

void Shader::set_vec4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader) this->use();
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void Shader::set_vec4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader) this->use();
    glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}

void Shader::set_mat4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader) this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}