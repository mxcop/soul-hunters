#include "ghost.h"
#include "../engine/math/vec.h"

#include "player.h"

std::vector<Ghost> Ghost::ghosts;

bool Ghost::make(glm::vec2 pos, float hp)
{
	auto itr = std::find_if(
		ghosts.begin(), ghosts.end(),
		[](Ghost& ghost) {
			return ghost.is_active == false;
		}
	);
	if (itr != ghosts.end()) {
		/* Might not be the most efficient to override the memory like this... */
		*itr = Ghost(pos, hp, itr->id);

		return true;
	}

	bool has_space = ghosts.size() < ghosts.capacity();

	if (has_space) {
		ghosts.push_back(Ghost(pos, hp, ghosts.size()));
	}
	return has_space;
}

void Ghost::update_all(Player* player1, Player* player2, float dt)
{
	for (Ghost& ghost : ghosts) {
		if (ghost.is_active) ghost.update(*player1, *player2, dt);
	}
}

void Ghost::draw_all(SpriteRenderer& renderer)
{
	for (Ghost& ghost : ghosts) {
		if (ghost.is_active) ghost.draw(renderer);
	}
}

bool Ghost::deactivate(int id)
{
	auto itr = std::find_if(
		ghosts.begin(), ghosts.end(),
		[&](Ghost& ghost) {
			return ghost.id == id;
		}
	);
	if (itr != ghosts.end()) {
		itr->is_active = false;
		return true;
	}
	return false;
}

Ghost::Ghost(glm::vec2 pos, float hp, int id)
{
	this->pos = pos;
	this->hp = hp;
	this->id = id;

	//move_right = rand() % 2 == 0;
	move_right = false;
	speed = 2.0f + rand() % 8;
}

void Ghost::update(Player& player1, Player& player2, float dt)
{
	time += dt;

	float dist1 = vec::dist(this->pos, player1.get_pos());
	float dist2 = vec::dist(this->pos, player2.get_pos());

	/*if (dist1 < 0.1f || dist2 < 0.1f) {
		deactivate(id);
		return;
	}*/

	// TODO: If HP is 0 and 2nd player is sucking, then deactivate
	if (this->hp == 0.0f && player2.light_range_check(*this)) this->deactivate(id);

	glm::vec2 dir = vec::from_to(dist1 < dist2 ? player1.get_pos() : player2.get_pos(), this->pos);
	//glm::vec2 side = move_right ? glm::vec2(-dir.y, dir.x) : glm::vec2(dir.y, -dir.x);

	//this->pos += dir * (this->speed * (1.0f + std::sin(time * 5.0f) * 4.0f)) * dt;
	this->pos += dir * (this->speed * dt);
	//this->pos += side * (this->speed * (1.0f + std::sin(time * 5.0f) * 4.0f)) * dt;
}

void Ghost::draw(SpriteRenderer& renderer) const
{
	renderer.draw_sprite(ResourceManager::get_texture("ghost"), this->pos, { 2.0f, 2.0f });
}
