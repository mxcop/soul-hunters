#pragma once
#include <LDtkLoader/Project.hpp>

#include "shader.h"

class Tilemap
{
public:
	Tilemap(const std::vector<ldtk::Tile>& data, uint16_t width, uint16_t height);
	~Tilemap();

	/// <summary>
	/// Setup tilemap functionality, should be called before the Game starts.
	/// </summary>
	static void Setup();

private:
	static Shader shader;
	uint16_t width, height;

	/// Contains the tile data. (RGB, only the Red channel is used)
	uint8_t* tex_buf = nullptr;
};