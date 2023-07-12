#pragma once

#include "LDtkLoader/Project.hpp"

class Tilemap
{
public:
	Tilemap(const std::vector<ldtk::Tile>& data, uint16_t width, uint16_t height);

private:
	uint16_t width, height;

	uint8_t* tile_data = nullptr;
};