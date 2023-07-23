#include "game.h"
#include "../engine/resource-manager.h"

#include <filesystem>

#include "LDtkLoader/Project.hpp"
#include "../engine/renderer/tilemap.h"
#include "../engine/renderer/light.h"

#include "collider.h"

#include <stdlib.h>

#include "../engine/inputs.h"

using std::filesystem::path;

Game::Game(int width, int height):
	state(GAME_ACTIVE), width(width), height(height)
{
}

Tilemap* test_map;
Light* test_light;
//Collider* test_collider;
Collider* test_player;

Game::~Game()
{
	delete test_map;
}

std::string relative_path(path p)
{
	return std::filesystem::absolute(p).string();
}

void Game::init()
{
	// Renderer setup functions:
	SpriteRenderer::setup("sprite");
	Tilemap::setup();
	Light::setup();

	// Load some level data:
	ldtk::Project ldtk_project;
	ldtk_project.loadFromFile("./public/test.ldtk");
	const auto& world = ldtk_project.getWorld();
	const auto& level = world.getLevel("Level_0");
	const auto& layer = level.getLayer("Background");
	const auto& layer2 = level.getLayer("Walls");
	const auto& tiles_vector = layer.allTiles();
	ldtk::IntPoint tilemap_size = layer.getGridSize();

	glm::vec2 level_offset = { level.position.x / 8.0f, level.position.y / 8.0f };

	// -----------------------------------------------------------------------
	for (const auto& wall : layer2.allEntities())
	{
		Collider::make({ -level_offset.x + (wall.getPosition().x / 8.0f), -level_offset.y - (wall.getPosition().y / 8.0f) }, { wall.getSize().x / 8.0f, wall.getSize().y / 8.0f});
	}

	// Set up projection matrix
	glm::mat4 projection = glm::ortho(0.0f - 4.0f, static_cast<float>(this->width - 4) / 30.0f, 0.0f, static_cast<float>(this->height) / 30.0f, 0.0f, 1000.0f);

	// Set the projection
	renderer.set_projection(projection, "sprite");
	test_map->set_projection(projection);
	
	// Load texture
	ResourceManager::load_texture(relative_path("./public/test/simplified/Level_0/_composite.png").c_str(), true, "test");
	ResourceManager::load_texture(relative_path("./public/awesomeface.png").c_str(), true, "bor");
	ResourceManager::load_texture(relative_path("./public/test-tileset.png").c_str(), true, "tileset");

	// Create a tilemap.
	test_map = new Tilemap(tiles_vector, ResourceManager::get_texture("tileset"), 5, tilemap_size.x, tilemap_size.y);
	
	// Create a light.
	test_light = new Light({ 0.0f, 0.0f }, 10.0f);
	test_light->set_projection(projection);

	// Create a test collider
	//test_collider = &Collider::make({ 0.0f, 0.0f }, { 2.0f, 2.0f });
	test_player = &Collider::make({ 0.0f, 0.0f }, { 2.0f, 2.0f });
}

bool up = false;
bool down = false;
bool left = false;
bool right = false;

void Game::key_input(int key, int action)
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

void Game::joystick_input(const float* axes, const unsigned char* buttons)
{
	movement.x = axes[LEFT_STICK_X];
	movement.y = -axes[LEFT_STICK_Y];

}

float speed = 10.0f;
float rotation = 0;

void Game::update(float dt)
{
	// Get the state of the axes
	int axes_count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);

	// Get the state of the buttons
	int button_count;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);

	if (axes != nullptr && buttons != nullptr)
		Game::joystick_input(axes, buttons);

	glm::vec2 vel = { 0, 0 };

	if (up) vel.y += speed;
	if (down) vel.y -= speed;
	if (left) vel.x -= speed;
	if (right) vel.x += speed;

	glm::vec2 normal = {};
	glm::vec2 collision_time = {};

	test_player->set_vel({ vel.x * dt, 0.0f });
	collision_time.x = test_player->swept_aabb(normal);
	test_player->set_vel({ 0.0f, vel.y * dt });
	collision_time.y = test_player->swept_aabb(normal);

	test_player->set_pos(test_player->get_pos() + vel * dt * collision_time);

	/*rotation += dt * 45.0f;
	if (rotation > 360.0f) rotation = 0.0f;*/

	test_light->set_pos(test_player->get_pos());
	// test_light->compute();
}

void Game::fixed_update() 
{
	test_light->compute();
}

void Game::render()
{
	test_map->draw(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	test_light->draw();

	renderer.draw_sprite(
		ResourceManager::get_texture("bor"),
		test_player->get_pos(),
		glm::vec2(2.0f, 2.0f),
		rotation);

	renderer.draw_sprite(
		ResourceManager::get_texture("tileset"),
		test_player->get_pos() + glm::vec2(80.0f * 5, 0),
		glm::vec2(1.0f * 5, 1.0f),
		rotation);
}