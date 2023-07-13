#pragma once
#include <LDtkLoader/Project.hpp>

#include "texture.h"
#include "shader.h"

class Tilemap
{
public:
	Tilemap() {};
	Tilemap(const std::vector<ldtk::Tile>& data, Texture2D tileset, uint8_t tiles, uint16_t width, uint16_t height);
	~Tilemap();

	/// <summary>
	/// Setup tilemap functionality, should be called before the Game starts.
	/// </summary>
	static void setup();

	void set_projection(glm::mat4 projection);
	void draw();

private:
	static Shader shader;

	/// The tileset texture.
	Texture2D tileset;
	uint8_t tileset_len = 0;

	GLuint vao = 0, vbo = 0, ebo = 0;
	uint16_t width = 0, height = 0;

	/// Contains the tile data. (RGB, only the Red channel is used)
	uint8_t* tex_buf = nullptr;
	Texture2D tile_data;

	/// <summary>
	/// Initialize the OpenGL render data.
	/// </summary>
	void gl_init();
};