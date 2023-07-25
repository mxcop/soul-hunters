#include "game.h"

#include <filesystem>
#include <stdlib.h>

#include "LDtkLoader/Project.hpp"

#include "../engine/resource-manager.h"
#include "../engine/renderer/tilemap.h"
#include "../engine/renderer/light.h"
#include "../engine/inputs.h"

#include "collider.h"


using std::filesystem::path;

Game::Game(int width, int height):
	state(GAME_ACTIVE), width(width), height(height)
{
}

Tilemap* test_map;
Light* test_light;

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
	SpriteRenderer::setup();
	Tilemap::setup();
	Light::setup();

	Collider::reserve_colliders(128);

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
	renderer.set_projection(projection);
	test_map->set_projection(projection);
	
	// Load texture
	ResourceManager::load_texture(relative_path("./public/test/simplified/Level_0/_composite.png").c_str(), true, "test");
	ResourceManager::load_texture(relative_path("./public/awesomeface.png").c_str(), true, "bor");
	ResourceManager::load_texture(relative_path("./public/test-tileset.png").c_str(), true, "tileset");

	this->player_1 = new Player({ 0.0f, 0.0f }, ResourceManager::load_texture(relative_path("./public/awesomeface.png").c_str(), true, "bor"), std::nullopt, this->keys);

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == 1) {
		this->player_2 = new Player({ 0.5f, 0.5f }, ResourceManager::load_texture(relative_path("./public/awesomeface.png").c_str(), true, "bor"), GLFW_JOYSTICK_1, this->keys);
	}

	// Create a tilemap.
	test_map = new Tilemap(tiles_vector, ResourceManager::get_texture("tileset"), 5, tilemap_size.x, tilemap_size.y);
	
	// Create a light.
	test_light = new Light({ 0.0f, 0.0f }, 10.0f);
	test_light->set_projection(projection);
}

void Game::key_input(int key, int action) {}

void Game::joystick_callback(int jid, int event)
{
	if (event == GLFW_CONNECTED)
	{
		this->joysticks[this->joystick_count++] = jid;
		if (this->player_2 != nullptr) {
			this->player_2->set_cid(jid);
		}
		else {
			player_2 = new Player({ 0.5f, 0.5f }, ResourceManager::load_texture(relative_path("./public/awesomeface.png").c_str(), true, "bor"), jid, this->keys);
		}
	}
	else if (event == GLFW_DISCONNECTED)
	{
		int i;

		for (i = 0; i < this->joystick_count; i++)
		{
			if (this->joysticks[i] == jid)
				break;
		}

		for (i = i + 1; i < this->joystick_count; i++)
			this->joysticks[i - 1] = this->joysticks[i];

		this->joystick_count--;

		if (this->joystick_count > 0) {
			int new_jid = this->joysticks[0];

			// TODO: Update second player CID here...
			player_2->set_cid(new_jid);
		}
	}
}

float speed = 10.0f;
float rotation = 0;

void Game::update(float dt)
{
	//// Get the state of the buttons
	//int button_count;
	//const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);

	//if (axes != nullptr && buttons != nullptr)
	//	Game::joystick_input(axes, buttons);

	/* Update the players */
	this->player_1->update(dt);

	if (this->player_2 != nullptr) {
		this->player_2->update(dt);
	}

	/* Move the testing light */
	test_light->set_pos(this->player_1->get_pos());
}

void Game::fixed_update() 
{
	/* Compute the shadow mask of the test light */
	test_light->compute();
}

void Game::render()
{
	test_map->draw(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	test_light->draw();

	/*renderer.draw_sprite(
		ResourceManager::get_texture("tileset"),
		glm::vec2(2, 2),
		glm::vec2(1.0f * 5, 1.0f),
		rotation);*/

	/* Draw the players */
	this->player_1->draw(renderer);

	if (this->player_2 != nullptr) {
		this->player_2->draw(renderer);
	}
}