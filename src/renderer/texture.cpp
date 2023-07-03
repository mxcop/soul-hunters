#include <iostream>

#include "texture.h"

Texture2D::Texture2D() : 
    width(0), height(0), 
    int_fmt(GL_RGB), img_fmt(GL_RGB), 
    wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), 
    filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{
    glGenTextures(1, &this->id);
}

void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    // Create the texture:
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->int_fmt, width, height, 0, this->img_fmt, GL_UNSIGNED_BYTE, data);

    // Set the wrap and filter modes:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);

    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}