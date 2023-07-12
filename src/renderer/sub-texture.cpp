#include "sub-texture.h"

SubTexture2D::SubTexture2D(const Texture2D& texture, glm::vec2 min, glm::vec2 max) :
	texture(texture)
{
	texture_coords[0] = {min.x, min.y};
	texture_coords[1] = {max.x, min.y};
	texture_coords[2] = {max.x, max.y};
	texture_coords[3] = {min.x, max.y};
}

SubTexture2D& SubTexture2D::create_from_coords(const Texture2D& texture, const glm::vec2 coords, const glm::vec2 sprite_size)
{
	glm::vec2 min = { (coords.x * sprite_size.x) / texture.width, (coords.y * sprite_size.y) / texture.height };
	glm::vec2 max = { ((coords.x + 1) * sprite_size.x) / texture.width, ((coords.y + 1) * sprite_size.y) / texture.height };

	return SubTexture2D(texture, min, max);
}