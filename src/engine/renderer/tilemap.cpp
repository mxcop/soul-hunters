#include <stdlib.h>

#include "tilemap.h"
#include "../resource-manager.h"

Shader Tilemap::shader;

Tilemap::Tilemap(const std::vector<ldtk::Tile>& data, Texture2D tileset, uint8_t tiles, uint16_t width, uint16_t height)
{
	this->tileset = tileset;
	tileset_len = tiles;
	this->width = width;
	this->height = height;

	/* Initialize the texture buffer */
	tex_buf = new uint8_t[width * height * 3] { 0u /* All elements 0 */ };

	/* Add all the tiles in the level into the texture buffer */
	for (const ldtk::Tile& tile : data)
	{
		ldtk::IntPoint pos = tile.getGridPosition();
		if (pos.x < width && pos.y < height) {
			tex_buf[(pos.y * width + pos.x) * 3] = tile.tileId + 1;
		}
	}

	/* Generate the tile data texture */
	tile_data.img_fmt = GL_RGB;
	tile_data.int_fmt = GL_RGB;
	tile_data.generate(width, height, tex_buf);

	gl_init();
}

void Tilemap::gl_init() 
{
	// Init objects
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	float vertices[] =
	{
		/*   x    */ /*    y    */
		0.0f,			height,
		width,			height,
		width,			0.0f,
		0.0f,			0.0f
	};

	// Upload the vertices to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Init more objects
	glGenBuffers(1, &ebo);

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Upload the indices (elements) to the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Pos atrribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}

Tilemap::~Tilemap()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	delete[] tex_buf;
}

void Tilemap::setup()
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
	shader.use();
	shader.set_int("tileset", 0); // GL_TEXTURE0
	shader.set_int("tile_data", 1); // GL_TEXTURE1
}

void Tilemap::set_projection(glm::mat4 projection)
{
	shader.use().set_mat4("projection", projection);
}

void Tilemap::draw(glm::vec2 position, glm::vec2 size)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Compute the model matrix:
	glm::mat4 model = glm::mat4(1.0f/* Identity matrix */);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

	// Set the uniforms within the shader:
	this->shader.use();
	shader.set_float("tileset_size", static_cast<float>(tileset_len));
	shader.set_vec2f("map_size", glm::vec2(width, height) * 1.0f);
	this->shader.set_mat4("model", model);

	// Bind the textures:
	tileset  .bind(0/* GL_TEXTURE0 */);
	tile_data.bind(1/* GL_TEXTURE1 */);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind the GL buffers:
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
