#include "game.h"

#include <filesystem>
#include <stdlib.h>
#include <imgui_widgets.cpp>

#include "LDtkLoader/Project.hpp"

#include "../engine/resource-manager.h"
#include "../engine/renderer/tilemap.h"
#include "../engine/renderer/light.h"
#include "../engine/inputs.h"

#include "collider.h"
#include "ghost.h"

int Game::score = 0;
using std::filesystem::path;

Game::Game(GLFWwindow* gl_window)
{
	this->state = GAME_MENU;
	this->gl_window = gl_window;
	glfwGetWindowSize(gl_window, &width, &height);
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

void Game::init()
{
	// Renderer setup functions:
	SpriteRenderer::setup();
	Tilemap::setup();
	Light::setup();

	Collider::reserve_colliders(128);
	Ghost::reserve_ghosts(256);

	// Load some level data:
	ldtk::Project ldtk_project;
	ldtk_project.loadFromFile("./public/level.ldtk");
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
		glm::vec2 wall_pos = { -level_offset.x + (wall.getPosition().x / 8.0f), -level_offset.y - (wall.getPosition().y / 8.0f) };
		glm::vec2 wall_size = { wall.getSize().x / 8.0f, wall.getSize().y / 8.0f };

		Collider::make(wall_pos, wall_size);

		// Left edge
		shadow_edges.push_back(wall_pos);
		shadow_edges.push_back({ wall_pos.x, wall_pos.y + wall_size.y });

		// Top edge
		shadow_edges.push_back(wall_pos);
		shadow_edges.push_back({ wall_pos.x + wall_size.x, wall_pos.y });

		// Right edge
		shadow_edges.push_back({ wall_pos.x + wall_size.x, wall_pos.y });
		shadow_edges.push_back({ wall_pos.x + wall_size.x, wall_pos.y + wall_size.y });

		// Bottom edge
		shadow_edges.push_back({ wall_pos.x + wall_size.x, wall_pos.y + wall_size.y });
		shadow_edges.push_back({ wall_pos.x, wall_pos.y + wall_size.y });
	}

	float aspect_ratio = this->width / this->height;

	// Set up projection matrix
	this->camera_center = glm::vec2(64.0f * aspect_ratio, 36.0f) / 2.0f;
	this->camera_size = glm::vec2(64.0f * aspect_ratio, 36.0f);
	this->projection = glm::ortho(0.0f, 64.0f * aspect_ratio, 0.0f, 36.0f, 0.0f, 1000.0f);

	// Set the projection
	renderer.set_projection(this->projection);
	test_map->set_projection(this->projection);
	
	// Load textures
	ResourceManager::load_texture(relative_path("./public/test/simplified/Level_0/_composite.png").c_str(), true, "test");
	ResourceManager::load_texture(relative_path("./public/player1.png").c_str(), true, "player1");
	ResourceManager::load_texture(relative_path("./public/player2.png").c_str(), true, "player2");
	ResourceManager::load_texture(relative_path("./public/player-hand.png").c_str(), true, "hand");
	ResourceManager::load_texture(relative_path("./public/player-vacuum.png").c_str(), true, "vacuum");
	ResourceManager::load_texture(relative_path("./public/player-flashlight.png").c_str(), true, "flashlight");
	ResourceManager::load_texture(relative_path("./public/dungeon-tileset.png").c_str(), true, "tileset");
	ResourceManager::load_texture(relative_path("./public/ghost-final.png").c_str(), true, "ghost");
	ResourceManager::load_texture(relative_path("./public/flashed-ghost.png").c_str(), true, "flashed ghost");

	this->digits = ResourceManager::load_texture(relative_path("./public/digits.png").c_str(), true, "digits");
	this->heart = ResourceManager::load_texture(relative_path("./public/heart.png").c_str(), true, "heart");

	this->game_title = ResourceManager::load_texture(relative_path("./public/game-title.png").c_str(), true, "game title");
	this->game_subtext = ResourceManager::load_texture(relative_path("./public/game-subtext.png").c_str(), true, "game subtext");

	this->player_1 = new Player(camera_center, ResourceManager::get_texture("player1"), ResourceManager::get_texture("hand"), ResourceManager::get_texture("flashlight"), std::nullopt, this->keys);
	this->player_1->set_projection(this->projection);

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == 1) {
		this->player_2 = new Player(camera_center, ResourceManager::get_texture("player2"), ResourceManager::get_texture("hand"), ResourceManager::get_texture("vacuum"), GLFW_JOYSTICK_1, this->keys);
		this->player_2->set_projection(this->projection);
	}

	Player::hp = Player::max_hp;

	// Create a tilemap.
	test_map = new Tilemap(tiles_vector, ResourceManager::get_texture("tileset"), 46, tilemap_size.x, tilemap_size.y);
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
			this->player_2 = new Player(camera_center, ResourceManager::get_texture("player2"), ResourceManager::get_texture("hand"), ResourceManager::get_texture("vacuum"), jid, this->keys);
			this->player_2->set_projection(this->projection);
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

			player_2->set_cid(new_jid);
		} else {
			player_2->set_cid(-1);
		}
	}
}

void Game::update(float dt)
{
	/* Check for start confirmation */
	if (this->state != GAME_ACTIVE) {
		int button_count;
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);

		if (buttons != nullptr) {
			if (buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS || buttons[GLFW_GAMEPAD_BUTTON_CROSS] == GLFW_PRESS) {
				/* Reset the score */
				this->score = 0;
				Player::hp = Player::max_hp;

				Ghost::clear();
				Ghost::make({ rand() % 80, rand() % 40 }, 5);

				this->state = GAME_ACTIVE;
			}
		}
	}

	if (Player::hp <= 0.0f) {
		this->state = GAME_OVER;
	}

	if (this->state != GAME_OVER) {
		/* Update the players */
		this->player_1->update(dt);

		if (this->player_2 != nullptr) {
			this->player_2->update(dt);
		}

		/* Update ghosts */
		Ghost::update_all(
			this->player_1,
			this->player_2,
			dt
		);
	}
}

void Game::fixed_update() 
{
	glfwGetWindowSize(gl_window, &width, &height);

	if (this->state != GAME_OVER) {
		player_1->fixed_update(gl_window, width, height, shadow_edges);

		if (this->player_2 != nullptr) {
			this->player_2->fixed_update(gl_window, width, height, shadow_edges);
		}
	}
}

void draw_score(SpriteRenderer& renderer, Texture2D& digits, glm::vec2 pos, float scale, int score) {
	/* Draw the score */
	int s = score;
	int i = 0;

	if (score == 0) {
		renderer.draw_sprite(
			digits,
			pos,
			glm::vec2(0.625f, 0.875f) * scale,
			0.0f, true, false,
			0, 10
		);
		return;
	}

	while (s > 0)
	{
		int digit = s % 10;

		renderer.draw_sprite(
			digits,
			pos - glm::vec2(i * (0.625f * scale), 0.0f),
			glm::vec2(0.625f, 0.875f) * scale,
			0.0f, true, false,
			digit, 10
		);

		s /= 10;
		i++;
	}
}

void Game::render()
{
	test_map->draw(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f));

	/* Draw the players */
	this->player_1->draw(renderer);

	if (this->player_2 != nullptr) {
		this->player_2->draw(renderer);
	}

	/* Draw based on game state */
	switch (this->state)
	{
	case GAME_ACTIVE:
		Ghost::draw_all(renderer);

		for (int i = 0; i < Player::max_hp; i++)
		{
			int frame = 0;

			/* Full heart */
			if (i + 1 <= Player::hp) {
				frame = 2;
			}

			/* Half heart */
			else if (i + 0.5f <= Player::hp) {
				frame = 1;
			}

			this->renderer.draw_sprite(
				this->heart,
				glm::vec2(camera_size.x - (Player::max_hp - i) * 1.25f * 1.25f, camera_size.y - 1.0f * 1.25f),
				glm::vec2(1.125f, 1.0f) * 1.25f,
				0.0f, true, false,
				frame, 3
			);
		}

		/* Draw the score */
		draw_score(this->renderer, this->digits, camera_size - glm::vec2(1.25f, 2.5f), 1.0f, this->score);
		break;

	case GAME_MENU:
		/* Menu graphics */
		this->renderer.draw_sprite(this->game_title, this->camera_center + glm::vec2(0.0f, 10.0f), glm::vec2(34.5f, 6.0f) / 1.5f);
		this->renderer.draw_sprite(this->game_subtext, camera_center + glm::vec2(0.0f, 5.0f), glm::vec2(16.5f, 1.75f) / 1.5f);
		break;

	case GAME_OVER:
		Ghost::draw_all(renderer);

		/* Menu graphics */
		this->renderer.draw_sprite(this->game_title, this->camera_center + glm::vec2(0.0f, 10.0f), glm::vec2(34.5f, 6.0f) / 1.5f);
		this->renderer.draw_sprite(this->game_subtext, camera_center + glm::vec2(0.0f, 5.0f), glm::vec2(16.5f, 1.75f) / 1.5f);

		/* Draw the score */
		float len = trunc(log10(this->score)) + 1.0f;
		draw_score(this->renderer, this->digits, camera_center + glm::vec2(len * 0.625f / 2.0f - 0.125f, 3.0f), 1.0f, this->score);
		break;
	}
}