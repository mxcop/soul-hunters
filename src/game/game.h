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
	
	bool keys[1024] = {0};
	int width, height;

	// Constructor/Destructor
	Game(int width, int height);

	/// <summary>
	/// Initialize game state (load shaders/textures)
	/// </summary>
	void Init();

	/// <summary>
	/// Process the input from the keyboard
	/// </summary>
	/// <param name="key">- Key pressed</param>
	/// <param name="action">- Action performed by player</param>
	void ProcessKeyInput(int key, int action);

	void ProcessJoystickInput(const float* axes, const unsigned char* buttons);

	/// <summary>
	/// Update game events
	/// </summary>
	/// <param name="dt">- delta time</param>
	void Update(float dt);
	
	/// <summary>
	/// Render the game
	/// </summary>
	void Render();

private:
	SpriteRenderer renderer;

	glm::vec2 pos = { 0.0, 0.0 };
};