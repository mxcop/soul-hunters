#include "light.h"
#include "../resource-manager.h"
#include <vector>
#include <string>

Shader Light::shader;

Light::Light(glm::vec2 pos, float range)
{
	this->pos = pos;
	this->range = range;

	gl_init();
}

void Light::setup()
{
	const std::string vert_src =
	#include "../shaders/debug.vert"
	;

	const std::string frag_src =
	#include "../shaders/debug.frag"
	;

	/* Load the shader program and save it */
	shader = ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, "debug");
}

// TEMP: for testing.
const std::vector<glm::vec2> wall_vertices = {
	{ -1.0f, -2.0f },
	{  1.0f, -2.0f },

	{ -1.0f, 1.0f },
	{  1.0f, 1.0f },

	{ -1.0f, 3.0f },
	{  1.0f, 3.0f },

	{ -1.0f, 1.0f },
	{ -1.0f, 3.0f },

	{  1.0f, 1.0f },
	{  1.0f, 3.0f },
};

void Light::compute()
{
	uint16_t wall_segments = wall_vertices.size() / 2;
	/* 12 vertices for each shadow quad */
	float* vertices = new float[12 * wall_segments];

	for (uint16_t i = 0; i < wall_segments; i++)
	{
		const glm::vec2& wall_vert1 = wall_vertices[i * 2];
		const glm::vec2& wall_vert2 = wall_vertices[i * 2 + 1];

		/* First triangle */
		vertices[i * 12 + 0] = wall_vert1.x;
		vertices[i * 12 + 1] = wall_vert1.y;

		vertices[i * 12 + 2] = wall_vert2.x;
		vertices[i * 12 + 3] = wall_vert2.y;

		vertices[i * 12 + 4] = wall_vert1.x + 10 * (wall_vert1.x - pos.x);
		vertices[i * 12 + 5] = wall_vert1.y + 10 * (wall_vert1.y - pos.y);

		/* Second triangle */
		vertices[i * 12 + 6] = wall_vert2.x;
		vertices[i * 12 + 7] = wall_vert2.y;

		vertices[i * 12 + 8] = wall_vert1.x + 10 * (wall_vert1.x - pos.x);
		vertices[i * 12 + 9] = wall_vert1.y + 10 * (wall_vert1.y - pos.y);

		vertices[i * 12 + 10] = wall_vert2.x + 10 * (wall_vert2.x - pos.x);
		vertices[i * 12 + 11] = wall_vert2.y + 10 * (wall_vert2.y - pos.y);
	}

	/* Write the computed shadow vertices into the buffers */
	glBindVertexArray(shadow.vao);
	glBindBuffer(GL_ARRAY_BUFFER, shadow.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * wall_segments, vertices, GL_DYNAMIC_DRAW);

	/* Unbind and delete */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] vertices;
}

void Light::draw()
{
	// TODO: move this to a fixed_update function running at the framerate of the game.
	compute();

    /* Enable the stencil buffer for drawing */
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF/* Start writing to the stencil */);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);

	/* Bind the shadow buffers */
	glBindVertexArray(shadow.vao);
	glBindBuffer(GL_ARRAY_BUFFER, shadow.vbo);

	/* Compute the model matrix */
	glm::mat4 model = glm::mat4(1.0f/* Identity matrix */);

	/* Draw the shadow mask onto the stencil buffer */
	this->shader.use();
	this->shader.set_vec4f("color", { 0.0f, 0.0f, 0.0f, 0.0f });
	this->shader.set_mat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 5 * 6);

	/* Reconfigure the stencil buffer to be used as a mask */
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00/* Stop writing to the stencil */);
    glDepthMask(GL_TRUE);

	/* Bind the light buffers */
	glBindVertexArray(light.vao);
	glBindBuffer(GL_ARRAY_BUFFER, light.vbo);

	/* Compute the model matrix */
	model = glm::translate(model, glm::vec3(pos, 0.0f));
	model = glm::scale(model, glm::vec3(15.0f, 15.0f, 1.0f));

	/* Draw the light with respect to the shadow mask */
	this->shader.use();
	this->shader.set_vec4f("color", { 1.0f, 1.0f, 0.7f, 0.5f });
	this->shader.set_mat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 2 * 6);

    /* Disable the stencil buffer */
    glDisable(GL_STENCIL_TEST);

	/* Unbind the buffers */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Light::set_pos(glm::vec2 pos)
{
	this->pos = pos;
}

void Light::set_projection(glm::mat4 projection)
{
	this->shader.use().set_mat4("projection", projection);
}

void Light::gl_init()
{
	/* Init & bind shadow buffers */
	glGenVertexArrays(1, &shadow.vao);
	glBindVertexArray(shadow.vao);
	glGenBuffers(1, &shadow.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, shadow.vbo);

	/* Declare shadow vertex attributes (pos) */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	/* Init & bind light buffers */
	glGenVertexArrays(1, &light.vao);
	glBindVertexArray(light.vao);
	glGenBuffers(1, &light.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, light.vbo);

	float vertices[12] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Declare light vertex attributes (pos) */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}
