#include "player.h"
#include <imgui.h>
#include "../engine/math/vec.h"

float Player::max_hp = 5.0f;
float Player::hp = 5.0f;

Player::Player(glm::vec2 initial_pos, Texture2D texture, Texture2D hand, Texture2D tool, std::optional<int> cid, bool* keys)
{
	this->initial_pos = initial_pos;
	this->texture = texture;
	this->hand = hand;
	this->tool = tool;
	this->keys = keys;
	this->collider = &Collider::make(this->initial_pos, { 1.5f, 1.5f });
	this->ambient_light = Light({ 0.0f, 0.0f }, 10.0f);
	this->ambient_light.color = { 1.0f, 1.0f, 0.7f, 0.8f };

	if (cid.has_value())
	{
		this->is_host = false;
		this->cid = cid.value();
		this->flashlight = Light({ 0.0f, 0.0f }, flashlight_range * 2.0f, flashlight_angle);
		this->flashlight.color = { 0.8f, 0.8f, 0.8f, 0.8f};
		return;
	}
	
	this->flashlight = Light({ 0.0f, 0.0f }, flashlight_range * 2.0f, flashlight_angle);
	this->flashlight.color = { 1.0f, 1.0f, 0.7f, 0.8f };
}

void Player::update(float dt)
{
	/* Move the player using user input */
	glm::vec2 vel = { 0.0f, 0.0f };

	if (this->is_host)
	{
		if (this->keys[GLFW_KEY_W]) vel.y += speed;
		if (this->keys[GLFW_KEY_S]) vel.y -= speed;
		if (this->keys[GLFW_KEY_A]) vel.x -= speed;
		if (this->keys[GLFW_KEY_D]) vel.x += speed;
	}
	else
	{
		/* Gamepad inputs */
		GLFWgamepadstate state;
		if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
		{
			float x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
			float y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

			/* Deadspace of 0.2f */
			if (std::abs(x) > 0.2f || std::abs(y) > 0.2f) {
				vel.x = x;
				vel.y = -y;
				vel *= speed;
			}
		}
	}

	/* Animations */
	this->anim_timer += dt;

	if (vel != glm::vec2(0.0f, 0.0f))
	{
		this->flip_x = vel.x < 0.0f;
		if (this->anim_timer > this->anim_delay) {
			this->frame += 1;
			this->anim_timer = 0.0f;

			if (this->frame > this->frames) this->frame = 0;
		}
	}
	else this->frame = 0;

	/* Player dual axis collision */
	glm::vec2 collision_time = {};

	this->collider->set_vel({ vel.x * dt, 0.0f });
	collision_time.x = this->collider->swept_aabb();
	this->collider->set_vel({ 0.0f, vel.y * dt });
	collision_time.y = this->collider->swept_aabb();

	this->collider->set_pos(this->collider->get_pos() + vel * dt * collision_time);

	/* Check if any ghosts are within the player's flashlight range */
	glm::vec2 current_pos = { this->collider->get_pos() };
	glm::vec2 center = { (current_pos + 1.0f) };
	
	float light_angle = glm::radians(flashlight_angle / 2.0f);
	
	std::vector<Ghost>& ghosts = Ghost::get_ghosts();
	for (Ghost& ghost : ghosts)
	{
		// Check if the ghost is within range of the first player
		if (light_range_check(ghost) && this->is_host) {
			ghost.hp = std::max(0.0f, ghost.hp - dt * 1.5f);
		}
		if (vec::dist(ghost.get_pos(), this->collider->get_pos()) < 0.2f && ghost.hp > 0.0f) {
			Player::hp -= 0.5f;
			ghost.hp = 0.0f;
		}
	}
}

glm::vec3 view_to_world(int win_w, int win_h, int mouse_x, int mouse_y, glm::mat4 proj) {
	// Normalised device space
	double x = 2.0 * mouse_x / win_w - 1;
	double y = 2.0 * mouse_y / win_h - 1;

	// Homogeneous space
	glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);

	// Projection space
	glm::mat4 viewProjectionInverse = inverse(proj);
	glm::vec4 worldPos = viewProjectionInverse * screenPos;
	return glm::vec3(worldPos);
}

void Player::fixed_update(GLFWwindow* gl_window, int win_w, int win_h, std::vector<glm::vec2>& shadow_edges)
{
	/* Move the lights */
	flashlight.pos = this->get_pos() + glm::vec2((this->flip_hand ? -0.875f : 0.875f) + 0.75f, 0.5f);
	ambient_light.pos = this->get_pos() + glm::vec2(0.75f, 0.75f);

	/* Aim the flashlight */
	if (is_host) {
		double mousex, mousey;
		glfwGetCursorPos(gl_window, &mousex, &mousey);

		glm::vec2 mouse = view_to_world(win_w, win_h, mousex, mousey, this->projection);
		glm::vec2 light_dir = mouse - flashlight.pos;
		glm::vec2 mouse_dir = mouse - this->get_pos();

		this->pointing_dir = flashlight.dir = vec::normalize(light_dir);

		this->flip_hand = mouse_dir.x < 0.0f;
	}
	else 
	{
		/* Gamepad inputs */
		GLFWgamepadstate state;
		if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
		{
			float x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
			float y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

			/* Deadspace of 0.2f */
			if (std::abs(x) > 0.2f || std::abs(y) > 0.2f) {
				glm::vec2 light_dir = { x, -y };
				this->pointing_dir = flashlight.dir = vec::normalize(light_dir);
				this->flip_hand = this->pointing_dir.x < 0.0f;
			}
		}
	}

	/* Compute the shadow masks */
	this->ambient_light.compute(shadow_edges);
	this->flashlight.compute(shadow_edges);
}

void Player::draw(SpriteRenderer& renderer)
{
	this->ambient_light.draw();
	this->flashlight.draw();

	glm::vec2 draw_pos = this->get_pos() + glm::vec2(0.75f, 1.0f);

	renderer.draw_sprite(
		this->texture,
		draw_pos + glm::vec2(this->flip_x ? 0.2f : -0.2f, 0.0f),
		glm::vec2(1.875f, 2.0f), 0, true, this->flip_x, this->frame, this->frames);

	renderer.draw_sprite(
		this->tool,
		draw_pos + glm::vec2(this->flip_hand ? -0.875f : 0.875f, -0.5f),
		glm::vec2(1.25f, 0.75f),
		atan2(this->pointing_dir.y, this->pointing_dir.x));

	renderer.draw_sprite(
		this->hand,
		draw_pos + glm::vec2(this->flip_hand ? -0.875f : 0.875f, -0.5f),
		glm::vec2(0.5f, 0.5f),
		atan2(this->pointing_dir.y, this->pointing_dir.x));
}

void Player::set_cid(int cid)
{
	this->cid = cid;
}

void Player::set_projection(glm::mat4 projection)
{
	this->projection = projection;
	this->flashlight.set_projection(projection);
	this->ambient_light.set_projection(projection);
}

bool Player::light_range_check(Ghost& ghost)
{
	/* Check if any ghosts are within the player's flashlight range */
	glm::vec2 current_pos = { this->collider->get_pos() };
	glm::vec2 center = { (current_pos + 1.0f) };

	float light_angle = glm::radians(flashlight_angle / 2.0f);
	
	if (vec::dist(center, ghost.get_pos()) >= flashlight.range / 2.0f) {
		return false;
	}

	/* Check if the ghost is within the flashlight angle */
	glm::vec2 ghost_dir = vec::normalize(current_pos - ghost.get_pos());
	float angle = std::acos(ghost_dir.x * -this->pointing_dir.x + ghost_dir.y * -this->pointing_dir.y);

	return angle < light_angle;
}