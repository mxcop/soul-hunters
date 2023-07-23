#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../engine/renderer/sprite-renderer.h"

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
	Game(int width, int height);
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

	/// <summary>
	/// Process the input from the joystick
	/// </summary>
	/// <param name="axes">- Axes data</param>
	/// <param name="buttons">- Buttons data</param>
	void joystick_input(const float* axes, const unsigned char* buttons);

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
	SpriteRenderer renderer;
};