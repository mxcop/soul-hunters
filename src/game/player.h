#pragma once

#include <optional>

#include "glm/glm.hpp"

#include "collider.h"

#include "../engine/renderer/texture.h"
#include "../engine/renderer/sprite-renderer.h"
#include "../engine/inputs.h"

class Player
{
public:
	Player(glm::vec2 initial_pos, Texture2D texture, std::optional<int> cid, bool* keys);

	void update(float dt);
	void draw(SpriteRenderer& renderer);

	void set_cid(int cid);

	glm::vec2 get_pos() { return this->collider->get_pos(); };
private:
	const float speed = 10.0f; 
	
	Collider* collider = nullptr;
	Texture2D texture;

	glm::vec2 initial_pos = {};
	bool is_host = true;
	int cid = 0u;
	bool* keys = nullptr;
};