#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "resource-manager.h"

class SpriteRenderer
{
public:
	SpriteRenderer();
	
	~SpriteRenderer();

	/// <summary>
	/// Draw the Sprite on the screen
	/// </summary>
	/// <param name="texture">- The texture of the Sprite</param>
	/// <param name="position">- The position where the Sprite will be drawn</param>
	/// <param name="size">- The size of the drawn Sprite</param>
	/// <param name="rotate">- The rotation value</param>
	/// <param name="color">- The color value</param>
	void draw_sprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, bool bind_tex = true);

	/// <summary>
	/// Load the shaders
	/// </summary>
	/// <param name="shader_name">- Set the name of the shader</param>
	static void setup(std::string shader_name);

	/// <summary>
	/// Set the projection
	/// </summary>
	/// <param name="projection">- The projection matrix</param>
	/// <param name="shader_name">- The name of the shader</param>
	static void set_projection(glm::mat4 projection, std::string shader_name);
private:
	static Shader shader;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	/// <summary>
	/// Initialises the vao, vbo and ebo of the Sprite to be rendered
	/// </summary>
	void init_renderdata();
};