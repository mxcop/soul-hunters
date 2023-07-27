#pragma once
#include <glad/glad.h>

/// <summary>
/// OpenGL 2D Texture helper.
/// </summary>
class Texture2D
{
    /* Dimensions in pixels */
    uint32_t width = 0, height = 0;

    /* Texture config */
    uint32_t wrap_s = GL_REPEAT; /* Wrapping mode on S axis */
    uint32_t wrap_t = GL_REPEAT; /* Wrapping mode on T axis */
    uint32_t filter_min = GL_NEAREST; /* Filtering mode if texture pixels < screen pixels */
    uint32_t filter_max = GL_NEAREST; /* Filtering mode if texture pixels > screen pixels */

public:
    uint32_t id = 0;

    uint32_t int_fmt = GL_RGB; /* Internal format */
    uint32_t img_fmt = GL_RGB; /* Image format */

    Texture2D(uint32_t img_fmt = GL_RGB);

    /// <summary>
    /// Upload a texture to the GPU for drawing.
    /// </summary>
    /// <param name="width">Width of the texture in pixels.</param>
    /// <param name="height">Height of the texture in pixels.</param>
    /// <param name="data">Texture data within RGB format.</param>
    void generate(uint32_t width, uint32_t height, uint8_t* data);

    /// <summary>
    /// Bind texture as active GL_TEXTURE_2D object
    /// </summary>
    /// <param name="tex_idx">GL_TEXTURE index.</param>
    void bind(int tex_idx = 0) const;
};