#include "resource-manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;


Shader ResourceManager::load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name)
{
    shaders[name] = load_shader_from_file(v_shader_file, f_shader_file, g_shader_file);
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

Shader ResourceManager::load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file)
{
    // 1. retrieve the vertex/fragment source code from filePath
    //std::string vertex_code;
    //std::string fragment_code;
    //std::string geometry_code;
    //try
    //{
    //    // open files
    //    std::ifstream vertex_shader_file(v_shader_file);
    //    std::ifstream fragment_shader_file(f_shader_file);
    //    std::stringstream v_shader_stream, f_shader_stream;
    //    // read file's buffer contents into streams
    //    v_shader_stream << vertex_shader_file.rdbuf();
    //    f_shader_stream << fragment_shader_file.rdbuf();
    //    // close file handlers
    //    vertex_shader_file.close();
    //    fragment_shader_file.close();
    //    // convert stream into string
    //    vertex_code = v_shader_stream.str();
    //    fragment_code = f_shader_stream.str();
    //    // if geometry shader path is present, also load a geometry shader
    //    if (g_shader_file != nullptr)
    //    {
    //        std::ifstream geometry_shader_file(g_shader_file);
    //        std::stringstream g_shader_stream;
    //        g_shader_stream << geometry_shader_file.rdbuf();
    //        geometry_shader_file.close();
    //        geometry_code = g_shader_stream.str();
    //    }
    //}
    //catch (std::exception e)
    //{
    //    std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    //}
    const char* v_shader_code = v_shader_file;
    const char* f_shader_code = f_shader_file;
    const char* g_shader_code = g_shader_file;
    // 2. now create shader object from source code
    Shader shader;
    assert(shader.compile(v_shader_code, f_shader_code, g_shader_file != nullptr ? g_shader_code : nullptr));
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.int_fmt = GL_RGBA;
        texture.img_fmt = GL_RGBA;
    }
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