#include "sprite-renderer.h"

Shader SpriteRenderer::shader;

SpriteRenderer::SpriteRenderer()
{
	this->init_renderdata();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void SpriteRenderer::draw_sprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, bool bind_tex)
{
	glm::mat4 model = glm::mat4(1.0f/* Identity matrix */);

	// Move the model to it's world position.
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// Rotate the model around it's center point. (hard coded center pivot)
	 //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	 model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	 //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

	// Scale the model.
	model = glm::scale(model, glm::vec3(size, 1.0f));

	// Set the uniforms within the shader:
	this->shader.use();
	this->shader.set_mat4("model", model);
	
	if (bind_tex)
	{
		glActiveTexture(GL_TEXTURE0);
		texture.bind();
	}

	//glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}

void SpriteRenderer::setup(std::string shader_name)
{
	const std::string vert_src =
	#include "../src/shaders/sprite.vert"
	;

	const std::string frag_src =
	#include "../src/shaders/sprite.frag"
	;

	// Load in shaders
	shader = ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, shader_name);

	// Set up shaders
	shader.use().set_int("img", 0);
}

void SpriteRenderer::set_projection(glm::mat4 projection, std::string shader_name)
{
	shader.set_mat4("projection", projection);
}

void SpriteRenderer::init_renderdata()
{
	// Init objects
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	float vertices[] =
	{
		// x   y      u     v
		-0.5f, 0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	// Tex attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(GLfloat)));
}