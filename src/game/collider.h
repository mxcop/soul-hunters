#pragma once

#include <glm/glm.hpp>
#include <vector>

class Collider
{
public:
	//Collider(Collider const&) = delete;

	/// <summary>
	///	Creates a vector of Collider Objects with the given position and size
	/// </summary>
	/// <param name="pos">- The position of the Collider</param>
	/// <param name="size">- The size of the Collider</param>
	/// <returns>Collider ref</returns>
	static Collider& make(glm::vec2 pos, glm::vec2 size);

	/// <summary>
	/// Checks if the current Collider Object is colliding with another Collider Object
	/// </summary>
	/// <returns>True if there is collision, False if there is no collision</returns>
	bool check();

	/// <summary>
	/// Set the position of the Collider Object
	/// </summary>
	/// <param name="pos">- The position of the Collider</param>
	void set_pos(glm::vec2 pos);

	/// <summary>
	/// Set the velocity of the Collider Object
	/// </summary>
	/// <param name="vel">- The velocity of the Collider</param>
	void set_vel(glm::vec2 vel);
	
	/// <summary>
	/// Gets the position of the Collider Object
	/// </summary>
	/// <returns>The position of the Collider</returns>
	glm::vec2 get_pos();

	/// <summary>
	/// Gets the velocity of the Collider Object
	/// </summary>
	/// <returns>The velocity of the Collider</returns>
	glm::vec2 get_vel();

	/// <summary>
	/// Swept AABB Collision check
	/// </summary>
	/// <param name="normal">- The normal of the static Collider Object</param>
	/// <returns> A number between 0 and 1 that indicates when the collision occurred. A value of 0 indicates the start of the movement and 1 indicates the end. If we get a value of 1, we can assume that there was no collision. A value of 0.5 means that the collision occurred halfway through the frame. </returns>
	float swept_aabb();
	
	static void reserve_colliders(size_t num) { colliders.reserve(num); };

private:
	Collider(glm::vec2 pos, glm::vec2 size, int id);

	glm::vec2 pos, size, velocity = {};
	float top, right;
	int id;


	static std::vector<Collider> colliders;

	/// <summary>
	/// Updates the bounds of the Collider Object
	/// </summary>
	void update_bounds();
};