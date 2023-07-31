#include "ghost.h"
#include "../engine/math/vec.h"

#include "player.h"
#include "game.h"

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
		if (ghost.is_active) ghost.update(player1, player2, dt);
	}
}

void Ghost::draw_all(SpriteRenderer& renderer)
{
	for (Ghost& ghost : ghosts) {
		if (ghost.is_active) ghost.draw(renderer);
	}
}

void Ghost::clear()
{
	for (Ghost& ghost : ghosts) {
		if (ghost.is_active) Ghost::deactivate(ghost.id);
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
	this->max_hp = hp;
	this->id = id;

	speed = 3.0f + rand() % 4;
}

void Ghost::update(Player* player1, Player* player2, float dt)
{
	if (player2 == nullptr) return;

	/* Animate */
	this->anim_timer += dt;
	if (this->anim_timer > std::min(0.2f, this->anim_delay / this->hp)) {
		this->frame += 1;
		this->anim_timer = 0.0f;

		if (this->frame > this->frames) this->frame = 0;
	}

	float dist1 = vec::dist(this->pos, player1->get_pos());
	float dist2 = vec::dist(this->pos, player2->get_pos());

	/* Suck it */
	if (this->hp <= 0.0f && player2->light_range_check(*this))
	{
		this->speed = speed + dt;
		glm::vec2 dir = vec::from_to(player2->get_pos(), this->pos);
		this->pos += dir * (this->speed * dt);

		if (vec::dist(this->pos, player2->get_pos()) < 0.5f) {
			this->deactivate(id);
			Game::score += 10;

			bool side = rand() % 2;

			if (side) {
				Ghost::make({ -3.0f, rand() % 36 }, 5);
				Ghost::make({ 67.0f, rand() % 36 }, 5);
			} else {
				Ghost::make({ rand() % 64, -3.0f }, 5);
				Ghost::make({ rand() % 64, 39.0f }, 5);
			}

			//Ghost::make({ rand() % 2 * 70.0f - 3.0f, rand() % 2 * 42.0f - 3.0f }, 5);
			//Ghost::make({ rand() % 2 * 70.0f - 3.0f, rand() % 2 * 42.0f - 3.0f }, 5);
		}
		return;
	}
	
	/* Get movement direction */
	glm::vec2 dir = vec::from_to(dist1 < dist2 ? player1->get_pos() : player2->get_pos(), this->pos);
	this->flip_x = dir.x < 0.0f;

	this->pos += dir * ((this->speed * (hp / max_hp)) * dt);
}

void Ghost::draw(SpriteRenderer& renderer) const
{
	renderer.draw_sprite(
		ResourceManager::get_texture(hp <= 0.0f ? "flashed ghost" : "ghost"), 
		this->pos + glm::vec2(0.8f, 1.0f),
		{ 1.6f, 2.0f }, 
		0.0f, 
		true,
		this->flip_x,
		frame, 
		frames
	);
}