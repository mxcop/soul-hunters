#include "ghost.h"
#include "../engine/math/vec.h"

Lake<Ghost> Ghost::ghosts;

Ghost::Ghost(glm::vec2 pos, float hp, int id)
{
	this->pos = pos;
	this->hp = hp;
	this->id = id;

	//move_right = rand() % 2 == 0;
	move_right = false;
	speed = 2.0f + rand() % 8;
}

void Ghost::update(glm::vec2 player1, glm::vec2 player2, float dt)
{
	time += dt;

	float dist1 = vec::dist(this->pos, player1);
	float dist2 = vec::dist(this->pos, player2);

	/*if (dist1 < 0.1f || dist2 < 0.1f) {
		deactivate(id);
		return;
	}*/

	if (this->hp <= 0.0f) {
		ghosts.remove(id);
		Ghost::make({ rand() % 80, rand() % 40 }, 5);
		return;
	}

	glm::vec2 dir = vec::from_to(dist1 < dist2 ? player1 : player2, this->pos);
	//glm::vec2 side = move_right ? glm::vec2(-dir.y, dir.x) : glm::vec2(dir.y, -dir.x);

	//this->pos += dir * (this->speed * (1.0f + std::sin(time * 5.0f) * 4.0f)) * dt;
	this->pos += dir * (this->speed * dt);
	//this->pos += side * (this->speed * (1.0f + std::sin(time * 5.0f) * 4.0f)) * dt;
}

void Ghost::draw(SpriteRenderer& renderer) const
{
	renderer.draw_sprite(ResourceManager::get_texture("ghost"), this->pos, { 2.0f, 2.0f });
}
