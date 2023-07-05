#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	/// Process user input through keys array
	/// </summary>
	/// <param name="dt">delta time</param>
	void ProcessInput(float dt);

	/// <summary>
	/// Update game events
	/// </summary>
	/// <param name="dt">delta time</param>
	void Update(float dt);
	
	/// <summary>
	/// Render the game
	/// </summary>
	void Render();
};