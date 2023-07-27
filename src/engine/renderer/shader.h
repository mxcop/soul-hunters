#pragma once
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// OpenGL Shader helper.
/// </summary>
class Shader
{
public:
    GLuint id = 0u;
    Shader() {}
    
    /// <summary>
    /// Select as current shader for rendering.
    /// </summary>
    Shader& use();

    /// <summary>
    /// Compile the given source files into a shader program.
    /// </summary>
    /// <param name="vert_src">The vertex shader source code</param>
    /// <param name="frag_src">The fragment shader source code</param>
    /// <param name="geo_src">The geometry shader source code (Optional)</param>
    /// <returns>False if an error has occured</returns>
    bool compile(const char* vert_src, const char* frag_src, const char* geo_src = nullptr);

    /* Uniform utility functions */
    void set_float(const char* name, float value, bool useShader = false);
    void set_int(const char* name, int value, bool useShader = false);
    void set_vec2f(const char* name, float x, float y, bool useShader = false);
    void set_vec2f(const char* name, const glm::vec2& value, bool useShader = false);
    void set_vec3f(const char* name, float x, float y, float z, bool useShader = false);
    void set_vec3f(const char* name, const glm::vec3& value, bool useShader = false);
    void set_vec4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void set_vec4f(const char* name, const glm::vec4& value, bool useShader = false);
    void set_mat4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
    /// <summary>
    /// Log any compiler / linker errors if any occured
    /// </summary>
    /// <param name="shader_id">ID of the shader to log</param>
    /// <param name="type">The type of the shader to log</param>
    /// <returns>False if an error has occured</returns>
    bool log_err(unsigned int shader_id, std::string type) const;
};