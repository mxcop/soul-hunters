#include "resource-manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;


Shader ResourceManager::load_shader(const char* v_shader_code, const char* f_shader_code, const char* g_shader_code, std::string name)
{
    shaders[name] = load_shader_from_file(v_shader_code, f_shader_code, g_shader_code);
    return shaders[name];
}

Shader ResourceManager::get_shader(std::string name)
{
    return shaders[name];
}

Texture2D ResourceManager::load_texture(const char* file, bool alpha, std::string name)
{
    textures[name] = load_texture_from_file(file, alpha);
    return textures[name];
}

Texture2D ResourceManager::get_texture(std::string name)
{
    return textures[name];
}

void ResourceManager::clear()
{
    // Delete all shaders	
    for (auto iter : shaders)
        glDeleteProgram(iter.second.id);
    // Delete all textures
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.id);
}

Shader ResourceManager::load_shader_from_file(const char* v_shader_code, const char* f_shader_code, const char* g_shader_code)
{
    // Now create shader object from source code
    Shader shader;
    assert(shader.compile(v_shader_code, f_shader_code, g_shader_code != nullptr ? g_shader_code : nullptr));
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture(alpha ? GL_RGBA : GL_RGB);

    // load image
    int width, height, nr_channels;
    unsigned char* data = stbi_load(file, &width, &height, &nr_channels, 0);

    stbi_failure_reason();

    // now generate texture
    texture.generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}