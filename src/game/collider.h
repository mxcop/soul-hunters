#pragma once

#include <glm/glm.hpp>
#include <vector>

class Collider
{
public:
	//Collider(Collider const&) = delete;

	static Collider& make(glm::vec2 pos, glm::vec2 size);

	bool check();
	void set_pos(glm::vec2 pos);

private:
	Collider(glm::vec2 pos, glm::vec2 size, int id);

	glm::vec2 pos, size;
	float top, right;
	int id;

	static std::vector<Collider> colliders;

	void update_bounds();
};