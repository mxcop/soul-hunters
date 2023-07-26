#include <iostream>

#include "texture.h"

Texture2D::Texture2D(uint32_t img_fmt) :
    int_fmt(img_fmt), img_fmt(img_fmt),
    wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), 
    filter_min(GL_NEAREST), filter_max(GL_NEAREST)
{
    glGenTextures(1, &this->id);
}

void Texture2D::generate(uint32_t width, uint32_t height, uint8_t* data, int tex_idx)
{
    this->width = width;
    this->height = height;

    glActiveTexture(GL_TEXTURE0 + tex_idx);

    // Create the texture:
    glBindTexture(GL_TEXTURE_2D, this->id);

    // Set the wrap and filter modes:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);

    glTexImage2D(GL_TEXTURE_2D, 0, this->int_fmt, width, height, 0, this->img_fmt, GL_UNSIGNED_BYTE, data);

    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind(int tex_idx) const
{
    glActiveTexture(GL_TEXTURE0 + tex_idx);
    glBindTexture(GL_TEXTURE_2D, this->id);
}