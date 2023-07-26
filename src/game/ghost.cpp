#include "ghost.h"

std::vector<Ghost> Ghost::ghosts;

Ghost& Ghost::make(glm::vec2 pos, float hp)
{
	assert(ghosts.size() < ghosts.capacity());

	ghosts.push_back(Ghost(pos, hp, ghosts.size()));

	return ghosts.back();
}

void Ghost::update_all(glm::vec2 player1, glm::vec2 player2, float dt)
{
	for (Ghost& ghost : ghosts) {
		ghost.update(player1, player2, dt);
	}
}

void Ghost::draw_all(SpriteRenderer& renderer)
{
	for (Ghost& ghost : ghosts) {
		ghost.draw(renderer);
	}
}

Ghost::Ghost(glm::vec2 pos, float hp, int id)
{
	this->pos = pos;
	this->hp = hp;
	this->id = id;

	move_right = rand() % 2 == 0;
	speed = 2.0f + rand() % 8;
}

/// <summary>
/// Get distance between two points.
/// </summary>
float dist(glm::vec2 a, glm::vec2 b) 
{
	glm::vec2 c = a - b;
	return sqrt(c.x * c.x + c.y * c.y);
}

/// <summary>
/// Get the normalized direction vector from a to b.
/// </summary>
glm::vec2 from_to(glm::vec2 a, glm::vec2 b)
{
	glm::vec2 c = a - b;
	float mag = sqrt(c.x * c.x + c.y * c.y);
	return c / std::max(0.000001f, mag);
}

void Ghost::update(glm::vec2 player1, glm::vec2 player2, float dt)
{
	time += dt;

	float dist1 = dist(this->pos, player1);
	float dist2 = dist(this->pos, player2);

	glm::vec2 dir = from_to(dist1 < dist2 ? player1 : player2, this->pos);
	glm::vec2 side = move_right ? glm::vec2(-dir.y, dir.x) : glm::vec2(dir.y, -dir.x);

	this->pos += dir * (this->speed * (1.0f + std::sin(time * 5.0f) * 4.0f)) * dt;
	this->pos += side * (this->speed * (1.0f + std::sin(time * 5.0f) * 4.0f)) * dt;
}

void Ghost::draw(SpriteRenderer& renderer)
{
	renderer.draw_sprite(ResourceManager::get_texture("ghost"), this->pos, { 2.0f, 2.0f });
}
