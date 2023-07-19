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
		if (this->pos.x >= other.left && this->pos.x <= other.right && this->pos.y >= other.top && this->pos.y <= other.bottom)
		{
			printf("TRUE\r\n\n");
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
	this->left = this->pos.x - this->size.x / 2;
	this->top = this->pos.y - this->size.y / 2;
	this->right = this->pos.x + this->size.x / 2;
	this->bottom = this->pos.y + this->size.y / 2;
}