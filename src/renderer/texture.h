#pragma once
#include <glad/glad.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particular texture.
    unsigned int id;

    // Texture image dimensions in pixels:
    unsigned int width, height;
    unsigned int int_fmt; // Internal format.
    unsigned int img_fmt; // Image format.

    // Texture config:
    unsigned int wrap_s; // Wrapping mode on S axis.
    unsigned int wrap_t; // Wrapping mode on T axis.
    unsigned int filter_min; // Filtering mode if texture pixels < screen pixels.
    unsigned int filter_max; // Filtering mode if texture pixels > screen pixels.

    Texture2D();

    /// <summary>
    /// Generate a texture from image data.
    /// </summary>
    /// <param name="width">- Width of the image in pixels</param>
    /// <param name="height">- Height of the image in pixels</param>
    /// <param name="data">- The image data (RGB)</param>
    void generate(unsigned int width, unsigned int height, unsigned char* data);

    /// <summary>
    /// Bind texture as active GL_TEXTURE_2D object
    /// </summary>
    void bind() const;
};
