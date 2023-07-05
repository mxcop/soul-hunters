#include "game.h"
#include "resource-manager.h"
#include "sprite-renderer.h"

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
	// Load in shaders
	ResourceManager::load_shader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "Sprite");

	// Set up shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().set_int("img", 0);
	ResourceManager::get_shader("sprite").set_mat4("projection", projection);

	// Set render controls
	renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

	// Load texture
	ResourceManager::load_texture("textures/awesomeface.png", true, "bor");
}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	renderer->draw_sprite(ResourceManager::get_texture("bor"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}