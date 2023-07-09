#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;

    /// <summary>
    /// Loads and generates a shader program from file loading the shader's source code
    /// </summary>
    /// <param name="v_shader_file">- Vertex Shader File Path</param>
    /// <param name="f_shader_file">- Fragment Shader File Path</param>
    /// <param name="g_shader_file">- Geometry Shader File Path</param>
    /// <param name="name">- Name</param>
    /// <returns>Shader Object</returns>
    static Shader load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name);
    
    /// <summary>
    /// Retrieves a shader
    /// </summary>
    /// <param name="name">- Name</param>
    /// <returns>Shader Object</returns>
    static Shader get_shader(std::string name);

    /// <summary>
    /// Loads and generates a texture from a file
    /// </summary>
    /// <param name="file">- File Path</param>
    /// <param name="alpha">- Alpha</param>
    /// <param name="name">- Name</param>
    /// <returns>2D Texture</returns>
    static Texture2D load_texture(const char* file, bool alpha, std::string name);

    /// <summary>
    /// Retrieves a texture
    /// </summary>
    /// <param name="name">- Name</param>
    /// <returns>2D Texture</returns>
    static Texture2D get_texture(std::string name);

    /// <summary>
    /// De-allocates loaded resources
    /// </summary>
    static void clear();
private:
    ResourceManager() { }

    /// <summary>
    /// Loads and generates a shader from a file
    /// </summary>
    /// <param name="v_shader_file">- Vertex Shader File Path</param>
    /// <param name="f_shader_file">- Fragment Shader File Path</param>
    /// <param name="g_shader_file">- Geometry Shader File Path</param>
    /// <returns>Shader Object</returns>
    static Shader load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file = nullptr);

    /// <summary>
    /// Loads a texture from a file
    /// </summary>
    /// <param name="file">- Texture File Path</param>
    /// <param name="alpha">- Alpha</param>
    /// <returns>2D Texture Object</returns>
    static Texture2D load_texture_from_file(const char* file, bool alpha);
};