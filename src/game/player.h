#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "collider.h"

#include "../engine/renderer/texture.h"
#include "../engine/renderer/sprite-renderer.h"

#include <optional>

class Player
{
public:
	Player(glm::vec2 initial_pos, Texture2D& texture, std::optional<int> cid, bool keys[]);

	void update(float dt);

private:
	Collider* collider;
	Texture2D texture;

	glm::vec2 initial_pos = {};
	const float speed = 10.0f; 
	bool is_host = true;
	int cid = 0u;
	bool* keys = nullptr;
};