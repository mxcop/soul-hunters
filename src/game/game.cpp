#include "game.h"
#include "../engine/resource-manager.h"

#include <filesystem>

#include "LDtkLoader/Project.hpp"
#include "../engine/renderer/tilemap.h"

#include <stdlib.h>

#include "../engine/inputs.h"

using std::filesystem::path;

Game::Game(int width, int height):
	state(GAME_ACTIVE), width(width), height(height)
{
}

Tilemap* test_map;

Game::~Game()
{
	delete test_map;
}

std::string relative_path(path p)
{
	return std::filesystem::absolute(p).string();
}

void Game::Init()
{
	// Renderer setup functions:
	SpriteRenderer::setup("sprite");
	Tilemap::setup();

	// Load some level data:
	ldtk::Project ldtk_project;
	ldtk_project.loadFromFile("./public/test.ldtk");
	const auto& world = ldtk_project.getWorld();
	const auto& level = world.getLevel("Level_0");
	const auto& layer = level.getLayer("Background");
	const auto& tiles_vector = layer.allTiles();
	ldtk::IntPoint tilemap_size = layer.getGridSize();

	// Set up projection matrix
	glm::mat4 projection = glm::ortho(-static_cast<float>(this->width) / 1.0f, static_cast<float>(this->width) / 1.0f, -static_cast<float>(this->height) / 1.0f, static_cast<float>(this->height) / 1.0f, 0.0f, 1000.0f);

	// Set the projection
	renderer.set_projection(projection, "sprite");
	test_map->set_projection(projection);
	
	// Load texture
	ResourceManager::load_texture(relative_path("./public/test/simplified/Level_0/_composite.png").c_str(), true, "test");
	ResourceManager::load_texture(relative_path("./public/awesomeface.png").c_str(), true, "bor");
	ResourceManager::load_texture(relative_path("./public/test-tileset.png").c_str(), true, "tileset");

	// Create a tilemap.
	test_map = new Tilemap(tiles_vector, ResourceManager::get_texture("tileset"), 5, tilemap_size.x, tilemap_size.y);

	pos = glm::vec2(-800.019470f * 2, -457.245911f * 2);
}

bool up = false;
bool down = false;
bool left = false;
bool right = false;

void Game::ProcessKeyInput(int key, int action)
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

glm::vec2 movement;

void Game::ProcessJoystickInput(const float* axes, const unsigned char* buttons)
{
	movement.x = axes[LEFT_STICK_X];
	movement.y = -axes[LEFT_STICK_Y];

}

float speed = 400.0f;
float rotation = 0;

void Game::Update(float dt)
{
	// Get the state of the axes
	int axes_count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);

	// Get the state of the buttons
	int button_count;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);

	if (axes != nullptr && buttons != nullptr)
		Game::ProcessJoystickInput(axes, buttons);

	if (up) pos.y += speed * dt;
	if (down) pos.y -= speed * dt;
	if (left) pos.x -= speed * dt;
	if (right) pos.x += speed * dt;

	pos += movement * dt * speed;

	/*rotation += dt * 45.0f;
	if (rotation > 360.0f) rotation = 0.0f;*/
}

void Game::Render()
{
	test_map->draw(glm::vec2(0.0f, 0.0f), glm::vec2(4.0f, 4.0f));

	renderer.draw_sprite(
		ResourceManager::get_texture("bor"),
		pos,
		glm::vec2(80.0f, 80.0f),
		rotation);

	renderer.draw_sprite(
		ResourceManager::get_texture("tileset"),
		pos + glm::vec2(80.0f * 5, 0),
		glm::vec2(80.0f * 5, 80.0f),
		rotation);
}