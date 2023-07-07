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

	std::filesystem::path p = "./src/textures/awesomeface.png";
	std::filesystem::path ap = std::filesystem::absolute(p);

	std::string ap_str = ap.string();
	const char* ap_str_const = ap_str.c_str();

	// Load in shaders
	ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, "sprite");

	// Set up shaders
	glm::mat4 projection = glm::ortho(-static_cast<float>(this->width) / 2.0f, static_cast<float>(this->width) / 2.0f, -static_cast<float>(this->height) / 2.0f, static_cast<float>(this->height) / 2.0f, 0.0f, 1000.0f);
	ResourceManager::get_shader("sprite").use().set_int("img", 0);
	ResourceManager::get_shader("sprite").set_mat4("projection", projection);

	// Set render controls
	renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

	// Load texture
	ResourceManager::load_texture(ap_str_const, true, "bor");

	glUniform1i(glGetUniformLocation(ResourceManager::get_shader("sprite").id, "sprite"), 0); // GL_TEXTURE0
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
}

bool up = false;
bool down = false;
bool left = false;
bool right = false;

void Game::ProcessInput(int key, int action)
{
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			up = true;
		}
		else if (action == GLFW_RELEASE) {
			up = false;
		}
	}

	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			down = true;
		}
		else if (action == GLFW_RELEASE) {
			down = false;
		}
	}

	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			left = true;
		}
		else if (action == GLFW_RELEASE) {
			left = false;
		}
	}

	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			right = true;
		}
		else if (action == GLFW_RELEASE) {
			right = false;
		}
	}
}

float speed = 400.0f;

void Game::Update(float dt)
{
	if (up) pos.y += speed * dt;
	if (down) pos.y -= speed * dt;
	if (left) pos.x -= speed * dt;
	if (right) pos.x += speed * dt;
}

void Game::Render()
{
	renderer->draw_sprite(
		ResourceManager::get_texture("bor"), 
		pos, 
		glm::vec2(100.0f, 100.0f), 
		0.0f, 
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
}