#include "sprite-renderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->shader = shader;
	this->init_renderdata();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void SpriteRenderer::draw_sprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	glm::mat4 model = glm::mat4(1.0f/* Identity matrix */);

	// Move the model to it's world position.
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

	// Rotate the model around it's center point. (hard coded center pivot)
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	// Scale the model.
	//model = glm::scale(model, glm::vec3(size, 1.0f));

	// Set the uniforms within the shader:
	this->shader.set_mat4("model", model);
	this->shader.set_vec3f("spriteColor", color);
	this->shader.use();

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	//glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);
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
		2, 3, 0,
		0, 1, 2,
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