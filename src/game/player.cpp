#include "player.h"
#include <imgui.h>
#include "ghost.h"
#include "../engine/vec.h"

Player::Player(glm::vec2 initial_pos, Texture2D texture, std::optional<int> cid, bool* keys)
{
	this->initial_pos = initial_pos;
	this->texture = texture;
	
	if (cid.has_value())
	{
		this->is_host = false;
		this->cid = cid.value();
	}
	this->keys = keys;

	this->collider = &Collider::make(this->initial_pos, {2.0f, 2.0f});

	this->flash_light = Light({ 0.0f, 0.0f }, flashlight_range * 2.0f, flashlight_angle);
	this->ambient_light = Light({ 0.0f, 0.0f }, 10.0f);
}

void Player::update(float dt)
{
	/* Move the player using user input */
	glm::vec2 vel = { 0, 0 };

	if (this->is_host)
	{
		if (this->keys[GLFW_KEY_W]) vel.y += speed;
		if (this->keys[GLFW_KEY_S]) vel.y -= speed;
		if (this->keys[GLFW_KEY_A]) vel.x -= speed;
		if (this->keys[GLFW_KEY_D]) vel.x += speed;
	}
	else
	{
		int axes_count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
		
		vel.x = axes[LEFT_STICK_X];
		vel.y = -axes[LEFT_STICK_Y];
		vel *= speed;
	}
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
		if (vec::dist(center, ghost.get_pos()) < flashlight_range) {
			glm::vec2 ghost_dir = vec::normalize(current_pos - ghost.get_pos());

			float angle = std::acos(ghost_dir.x * -this->pointing_dir.x + ghost_dir.y * -this->pointing_dir.y);
			bool in_range = angle < light_angle;

			if (in_range) ghost.hp = 0.0f;
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
	flash_light.pos = this->get_pos() + glm::vec2(1.0f, 1.0f);
	ambient_light.pos = flash_light.pos;

	if (is_host) {
		double mousex, mousey;
		glfwGetCursorPos(gl_window, &mousex, &mousey);

		glm::vec2 mouse = view_to_world(win_w, win_h, mousex, mousey, this->projection);
		glm::vec2 light_dir = mouse - flash_light.pos;

		this->pointing_dir = flash_light.dir = vec::normalize(light_dir);
	}
	else 
	{
		int axes_count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);

		glm::vec2 light_dir = { axes[RIGHT_STICK_X], -axes[RIGHT_STICK_Y] };

		this->pointing_dir = flash_light.dir = vec::normalize(light_dir);
	}

	/* Compute the shadow masks */
	this->ambient_light.compute(shadow_edges);
	this->flash_light.compute(shadow_edges);

	ImGui::Begin("Player");
	ImGui::SetWindowFontScale(1.5f);
	ImGui::Text("Position: %.1f, %.1f", get_pos().x, get_pos().y);
	ImGui::End();
}

void Player::draw(SpriteRenderer& renderer)
{
	this->ambient_light.draw();
	this->flash_light.draw();

	renderer.draw_sprite(
		this->texture,
		this->get_pos(),
		glm::vec2(2.0f, 2.0f), 0);
}

void Player::set_cid(int cid)
{
	this->cid = cid;
}

void Player::set_projection(glm::mat4 projection)
{
	this->projection = projection;
	this->flash_light.set_projection(projection);
	this->ambient_light.set_projection(projection);
}
