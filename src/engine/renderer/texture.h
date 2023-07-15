#pragma once
#include <glad/glad.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particular texture.
    uint32_t id = 0;

    // Texture image dimensions in pixels:
    uint32_t width = 0, height = 0;
    uint32_t int_fmt = GL_RGB; // Internal format.
    uint32_t img_fmt = GL_RGB; // Image format.

    // Texture config:
    uint32_t wrap_s = GL_REPEAT; // Wrapping mode on S axis.
    uint32_t wrap_t = GL_REPEAT; // Wrapping mode on T axis.
    uint32_t filter_min = GL_NEAREST; // Filtering mode if texture pixels < screen pixels.
    uint32_t filter_max = GL_NEAREST; // Filtering mode if texture pixels > screen pixels.

    Texture2D(uint32_t img_fmt = GL_RGB);

    /// <summary>
    /// Upload a texture to the GPU for drawing.
    /// </summary>
    /// <param name="width">Width of the texture in pixels.</param>
    /// <param name="height">Height of the texture in pixels.</param>
    /// <param name="data">Texture data within RGB format.</param>
    /// <param name="tex_idx">GL_TEXTURE index.</param>
    void generate(uint32_t width, uint32_t height, uint8_t* data, int tex_idx = 0);

    /// <summary>
    /// Bind texture as active GL_TEXTURE_2D object
    /// </summary>
    /// <param name="tex_idx">GL_TEXTURE index.</param>
    void bind(int tex_idx = 0) const;
};