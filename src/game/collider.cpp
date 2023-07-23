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

void Collider::set_vel(glm::vec2 vel)
{
	this->velocity = vel;
}

glm::vec2 Collider::get_pos()
{
	return this->pos;
}

glm::vec2 Collider::get_vel()
{
	return this->velocity;
}

float Collider::swept_aabb(glm::vec2& normal)
{
	
	float min = 1.0f;

	for (Collider& other : colliders)
	{
		glm::vec2 inv_entry = {}, inv_exit = {};
		glm::vec2 entry = {}, exit = {};

		if (other.id == this->id)
		{
			continue;
		}
		// find the distance between the objects on the near and far sides for both x and y 
		if (this->velocity.x > 0.0f)
		{
			inv_entry.x = other.pos.x - (this->pos.x + this->size.x);
			inv_exit.x = (other.pos.x + other.size.x) - this->pos.x;
		}
		else
		{
			inv_entry.x = (other.pos.x + other.size.x) - this->pos.x;
			inv_exit.x = other.pos.x - (this->pos.x + this->size.x);
		}

		if (this->velocity.y > 0.0f)
		{
			inv_entry.y = other.pos.y - (this->pos.y + this->size.y);
			inv_exit.y = (other.pos.y + other.size.y) - this->pos.y;
		}
		else
		{
			inv_entry.y = (other.pos.y + other.size.y) - this->pos.y;
			inv_exit.y = other.pos.y - (this->pos.y + this->size.y);
		}

		// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
		if (this->velocity.x == 0.0f)
		{
			entry.x = -std::numeric_limits<float>::infinity();
			exit.x = std::numeric_limits<float>::infinity();
		}
		else
		{
			entry.x = inv_entry.x / this->velocity.x;
			exit.x = inv_exit.x / this->velocity.x;
		}

		if (this->velocity.y == 0.0f)
		{
			entry.y = -std::numeric_limits<float>::infinity();
			exit.y = std::numeric_limits<float>::infinity();
		}
		else
		{
			entry.y = inv_entry.y / this->velocity.y;
			exit.y = inv_exit.y / this->velocity.y;
		}

		if (entry.y > 1.0f) entry.y = -std::numeric_limits<float>::infinity();
		if (entry.x > 1.0f) entry.x = -std::numeric_limits<float>::infinity();

		// find the earliest/latest times of collision 
		float entry_time = std::max(entry.x, entry.y);
		float exit_time = std::min(exit.x, exit.y);

		glm::vec2 this_obj_min = this->pos;
		glm::vec2 this_obj_max = this->pos + this->size;

		glm::vec2 other_obj_min = other.pos;
		glm::vec2 other_obj_max = other.pos + other.size;

		if (entry_time > exit_time) continue; // This check was correct.
		if (entry.x < 0.0f && entry.y < 0.0f) continue;

		if (entry.x < 0.0f)
		{
			// Check that the bounding box started overlapped or not.
			if (other_obj_max.x <= this_obj_min.x || other_obj_min.x >= this_obj_max.x) continue;
		}
		if (entry.y < 0.0f)
		{
			// Check that the bounding box started overlapped or not.
			if (other_obj_max.y <= this_obj_min.y || other_obj_min.y >= this_obj_max.y) continue;
		}

		// calculate normal of collided surface
		if (entry_time < min)
		{
			if (entry.x > entry.y)
			{
				if (inv_entry.x < 0.0f)
				{
					normal.x = 1.0f;
					normal.y = 0.0f;
				}
				else
				{
					normal.x = -1.0f;
					normal.y = 0.0f;
				}
			}
			else
			{
				if (inv_entry.y < 0.0f)
				{
					normal.x = 0.0f;
					normal.y = 1.0f;
				}
				else
				{
					normal.x = 0.0f;
					normal.y = -1.0f;
				}
			} // return the time of collision
			min = entry_time;
		}
	}
	return min;
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