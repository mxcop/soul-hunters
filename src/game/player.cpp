#include "player.h"
#include <GLFW/glfw3.h>

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
}

void Player::update(float dt)
{
	glm::vec2 vel = { 0, 0 };

	if (this->keys[GLFW_KEY_W]) vel.y += speed;
	if (this->keys[GLFW_KEY_S]) vel.y -= speed;
	if (this->keys[GLFW_KEY_A]) vel.x -= speed;
	if (this->keys[GLFW_KEY_D]) vel.x += speed;

	glm::vec2 collision_time = {};

	this->collider->set_vel({ vel.x * dt, 0.0f });
	collision_time.x = this->collider->swept_aabb();
	this->collider->set_vel({ 0.0f, vel.y * dt });
	collision_time.y = this->collider->swept_aabb();

	this->collider->set_pos(this->collider->get_pos() + vel * dt * collision_time);
}

void Player::draw(SpriteRenderer& renderer)
{
	renderer.draw_sprite(
		this->texture,
		this->get_pos(),
		glm::vec2(2.0f, 2.0f), 0);
}

void Player::set_cid(int cid)
{
	this->cid = cid;
}