#include "player.h"

Player::Player(glm::vec2 initial_pos, Texture2D& texture, std::optional<int> cid, bool keys[])
{
	this->initial_pos = initial_pos;
	this->texture = texture;
	
	if (cid.has_value())
	{
		this->is_host = false;
		this->cid = cid.value();
	}
	this->keys = keys;

	SpriteRenderer::setup();
	
	this->collider = &Collider::make(this->initial_pos, {2.0f, 2.0f});
}

void Player::update(float dt)
{
	glm::vec2 vel = { 0, 0 };

	if (this->keys[GLFW_KEY_UP]) vel.y += speed;
	if (this->keys[GLFW_KEY_DOWN]) vel.y -= speed;
	if (this->keys[GLFW_KEY_LEFT]) vel.x -= speed;
	if (this->keys[GLFW_KEY_RIGHT]) vel.x += speed;

	glm::vec2 collision_time = {};

	this->collider->set_vel({ vel.x * dt, 0.0f });
	collision_time.x = this->collider->swept_aabb();
	this->collider->set_vel({ 0.0f, vel.y * dt });
	collision_time.y = this->collider->swept_aabb();

	this->collider->set_pos(this->collider->get_pos() + vel * dt * collision_time);
}

void Player::set_cid(int cid)
{
	this->cid = cid;
}