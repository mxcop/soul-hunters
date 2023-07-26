#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../engine/renderer/sprite-renderer.h"

#include "player.h"

// Might want to add more depending on what we want later on
enum GameState 
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	// Game state
	GameState state;

	bool keys[1024] = { 0 };

	int width, height;

	// Constructor/Destructor
	Game(GLFWwindow* gl_window, int width, int height);
	~Game();

	/// <summary>
	/// Initialize game state (load shaders/textures)
	/// </summary>
	void init();

	/// <summary>
	/// Process the input from the keyboard
	/// </summary>
	/// <param name="key">- Key pressed</param>
	/// <param name="action">- Action performed by player</param>
	void key_input(int key, int action);

	void joystick_callback(int jid, int event);

	/// <summary>
	/// Update game events
	/// </summary>
	/// <param name="dt">- delta time</param>
	void update(float dt);

	/// <summary>
	/// Fixed update running at 60fps
	/// </summary>
	void fixed_update();
	
	/// <summary>
	/// Render the game
	/// </summary>
	void render();

private:
	GLFWwindow* gl_window = nullptr;
	SpriteRenderer renderer;
	Player* player_1 = nullptr;
	Player* player_2 = nullptr;
	glm::mat4 projection = {};

	std::vector<glm::vec2> shadow_edges;

	int joysticks[GLFW_JOYSTICK_LAST + 1];
	int joystick_count = 0;
};