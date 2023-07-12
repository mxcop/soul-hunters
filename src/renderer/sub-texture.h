#pragma once

#include "texture.h"
#include "glm/glm.hpp"

class SubTexture2D
{
public:
	SubTexture2D(const Texture2D& texture, glm::vec2 min, glm::vec2 max);

	const Texture2D get_texture() const { return texture; }
	const glm::vec2* get_tex_coords() const {return texture_coords; }

	static SubTexture2D& create_from_coords(const Texture2D& texture, const glm::vec2 coords, const glm::vec2 sprite_size);
private:
	const Texture2D texture;

	glm::vec2 texture_coords[4];
};