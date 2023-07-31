#pragma once

#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "collider.h"
#include "ghost.h"

#include "../engine/renderer/texture.h"
#include "../engine/renderer/sprite-renderer.h"
#include "../engine/inputs.h"
#include "../engine/renderer/light.h"

class Player
{
public:
	Player(glm::vec2 initial_pos, Texture2D texture, Texture2D hand, Texture2D tool, std::optional<int> cid, bool* keys);

	void update(float dt);
	void fixed_update(GLFWwindow* gl_window, int win_w, int win_h, std::vector<glm::vec2>& shadow_edges);
	void draw(SpriteRenderer& renderer);

	void set_cid(int cid);

	/// <summary>
	/// Set the projection matrix of the player.
	/// </summary>
	void set_projection(glm::mat4 projection);

	glm::vec2 get_pos() { return this->collider->get_pos(); };

	bool light_range_check(Ghost& ghost);

	static float max_hp;
	static float hp;
private:
	const float speed = 10.0f;

	/// Distance the light of the flashlight travels.
	const float flashlight_range = 15.0f;
	/// Angle of the flashlight in degrees.
	const float flashlight_angle = 70.0f;
	
	glm::mat4 projection = {};
	Collider* collider = nullptr;
	Texture2D texture;
	Texture2D hand;
	Texture2D tool;

	int frame = 0; 
	const int frames = 6;
	float anim_timer = 0.0f;
	const float anim_delay = 0.1f;
	bool flip_x = false;
	bool flip_hand = false;

	Light flashlight;
	Light ambient_light;

	glm::vec2 initial_pos = {};
	glm::vec2 pointing_dir = {};

	bool is_host = true;
	int cid = 0u;
	bool* keys = nullptr;
};