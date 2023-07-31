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
	GAME_OVER
};

class Game
{
public:
	// Game state
	GameState state;

	bool keys[1024] = { 0 };

	static int score;
	int width, height;

	// Constructor/Destructor
	Game(GLFWwindow* gl_window);
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

	Texture2D game_title;
	Texture2D game_subtext;
	Texture2D heart;
	Texture2D digits;

	int heart_frames = 3;
	int heart_frame = 0;

	SpriteRenderer renderer;

	Player* player_1 = nullptr;
	Player* player_2 = nullptr;

	glm::mat4 projection = {};

	glm::vec2 camera_size = {};
	glm::vec2 camera_center = {};

	std::vector<glm::vec2> shadow_edges;

	int joysticks[GLFW_JOYSTICK_LAST + 1];
	int joystick_count = 0;
};