#include "tilemap.h"
#include "../resource-manager.h"

Shader Tilemap::shader;

Tilemap::Tilemap(const std::vector<ldtk::Tile>& data, uint16_t width, uint16_t height) :
	width(width), height(height)
{
	/* Initialize the texture buffer */
	tex_buf = new uint8_t[width * height * 3] { 0u /* All elements 0 */ };

	/* Add all the tiles in the level into the texture buffer */
	for (const ldtk::Tile& tile : data)
	{
		ldtk::IntPoint pos = tile.getGridPosition();
		tex_buf[(pos.y * width + pos.x) * 3] = tile.tileId;
	}
}

Tilemap::~Tilemap()
{
	delete[] tex_buf;
}

void Tilemap::Setup()
{
	const std::string vert_src =
	#include "../shaders/tilemap.vert"
	;

	const std::string frag_src =
	#include "../shaders/tilemap.frag"
	;

	/* Load the tilemap shader program and save it */
	shader = ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, "tilemap");

	/* Set the map size */
	shader.use().set_vec2f("mapSize", glm::vec2(256, 256));
}
