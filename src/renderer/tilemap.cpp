#include "tilemap.h"

Tilemap::Tilemap(const std::vector<ldtk::Tile>& data, uint16_t width, uint16_t height) :
	width(width), height(height)
{
	tile_data = new uint8_t[width * height * 3];

	for (int i = 0; i < sizeof(tile_data); ++i)
	{
		tile_data[i] = 0;
	}

	for (const auto& tile : data)
	{
		ldtk::IntPoint pos = tile.getGridPosition();
		const int index = tile.tileId;
		tile_data[(pos.y * width + pos.x) * 3] = index;
	}


}