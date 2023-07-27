#pragma once

#include "glm/glm.hpp"
#include <algorithm>

namespace vec {
	/// <summary>
	/// Get distance between two points.
	/// </summary>
	inline float dist(glm::vec2 a, glm::vec2 b)
	{
		glm::vec2 c = a - b;
		return sqrt(c.x * c.x + c.y * c.y);
	}

	inline glm::vec2 normalize(glm::vec2 v)
	{
		float mag = sqrt(v.x * v.x + v.y * v.y);
		return v / std::max(0.000001f, mag);
	}

	/// <summary>
	/// Get the normalized direction vector from a to b.
	/// </summary>
	inline glm::vec2 from_to(glm::vec2 a, glm::vec2 b)
	{
		glm::vec2 c = a - b;
		float mag = sqrt(c.x * c.x + c.y * c.y);
		return c / std::max(0.000001f, mag);
	}
}
