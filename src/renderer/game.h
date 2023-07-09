#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
	
	bool keys[1024];
	unsigned int width, height;

	// Constructor/Destructor
	Game(unsigned int width, unsigned int height);
	~Game();

	/// <summary>
	/// Initialize game state (load shaders/textures)
	/// </summary>
	void Init();

	/// <summary>
	/// Process the input from the player
	/// </summary>
	/// <param name="key">- Key pressed</param>
	/// <param name="action">- Action performed by player</param>
	void ProcessInput(int key, int action);

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
	glm::vec2 pos = { 0.0, 0.0 };
};