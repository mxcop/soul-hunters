#include "game.h"
#include "resource-manager.h"
#include "sprite-renderer.h"

#include <filesystem>

SpriteRenderer* renderer;

Game::Game(unsigned int width, unsigned int height):
	state(GAME_ACTIVE), keys(), width(width), height(height)
{
}

Game::~Game()
{
	delete renderer;
}

void Game::Init()
{
	const std::string vert_src = 
	#include "../src/shaders/sprite.vert"
	;

	const std::string frag_src =
	#include "../src/shaders/sprite.frag"
	;

	std::filesystem::path p = "/awesomeface.png";
	std::filesystem::path ap = std::filesystem::absolute(p);

	std::string ap_str = ap.string();
	const char* ap_str_const = ap_str.c_str();

	// Load in shaders
	ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, "sprite");

	// Set up shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().set_int("img", 0);
	ResourceManager::get_shader("sprite").set_mat4("projection", projection);

	// Set render controls
	renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

	// Load texture
	ResourceManager::load_texture("C:/Users/gikst/Documents/Programming/premake-game/src/textures/awesomeface.png", true, "bor");
}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	renderer->draw_sprite(ResourceManager::get_texture("bor"), glm::vec2(200.0f, 200.0f), glm::vec2(30.0f, 40.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}