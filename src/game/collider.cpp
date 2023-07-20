#include "collider.h"

std::vector<Collider> Collider::colliders;

Collider& Collider::make(glm::vec2 pos, glm::vec2 size)
{
	colliders.push_back(Collider(pos, size, colliders.size()));

	return colliders.back();
}

bool Collider::check()
{ 
	for (Collider& other : colliders)
	{
		if (other.id == this->id)
		{
			continue;
		}
		float left = other.pos.x - (this->pos.x + this->size.x);
		float top = (other.pos.y + other.size.y) - this->pos.y;
		float right = (other.pos.x + other.size.x) - this->pos.x;
		float bottom = other.pos.y - (this->pos.y + this->size.y);

		if (!(left > 0 || right < 0 || top < 0 || bottom > 0))
		{
			return true;
		}
	}
	return false;
}

void Collider::set_pos(glm::vec2 pos)
{
	this->pos = pos;
	update_bounds();
}

Collider::Collider(glm::vec2 pos, glm::vec2 size, int id)
{
	this->pos = pos;
	this->size = size;
	this->id = id;
	update_bounds();
}

void Collider::update_bounds()
{
	this->top = this->pos.y + this->size.y;
	this->right = this->pos.x + this->size.x;
}