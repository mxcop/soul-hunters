#include <iostream>

#include "texture.h"

Texture2D::Texture2D(uint32_t img_fmt)
{
    this->int_fmt = img_fmt;
    this->img_fmt = img_fmt;
}

void Texture2D::generate(uint32_t width, uint32_t height, uint8_t* data)
{
    this->width = width;
    this->height = height;

    /* Create a new texture */
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    /* Set wrap and filter modes */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);

    /* Generate the image */
    glTexImage2D(GL_TEXTURE_2D, 0, this->int_fmt, width, height, 0, this->img_fmt, GL_UNSIGNED_BYTE, data);

    /* Cleanup */
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind(int tex_idx) const
{
    /* Bind our texture id */
    glActiveTexture(GL_TEXTURE0 + tex_idx);
    glBindTexture(GL_TEXTURE_2D, this->id);
}