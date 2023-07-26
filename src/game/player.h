#pragma once

#include <optional>

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "collider.h"

#include "../engine/renderer/texture.h"
#include "../engine/renderer/sprite-renderer.h"
#include "../engine/inputs.h"
#include "../engine/renderer/light.h"

class Player
{
public:
	Player(glm::vec2 initial_pos, Texture2D texture, std::optional<int> cid, bool* keys);

	void update(float dt);
	void fixed_update(GLFWwindow* gl_window, int win_w, int win_h, std::vector<glm::vec2>& shadow_edges);
	void draw(SpriteRenderer& renderer);

	void set_cid(int cid);

	/// <summary>
	/// Set the projection matrix of the tilemap.
	/// </summary>
	void set_projection(glm::mat4 projection);

	glm::vec2 get_pos() { return this->collider->get_pos(); };
private:
	const float speed = 10.0f; 
	
	glm::mat4 projection = {};
	Collider* collider = nullptr;
	Texture2D texture;

	Light flash_light;
	Light ambient_light;

	glm::vec2 initial_pos = {};
	bool is_host = true;
	int cid = 0u;
	bool* keys = nullptr;
};