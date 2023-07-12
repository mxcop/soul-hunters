#include "game.h"
#include "resource-manager.h"
#include "sprite-renderer.h"

#include <filesystem>

#include "LDtkLoader/Project.hpp"
#include "tilemap.h"

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
	ldtk::Project ldtk_project;
	ldtk_project.loadFromFile("./public/test.ldtk");

	// get the world
	const auto& world = ldtk_project.getWorld();

	// get the level and the layer we want to render
	const auto& level = world.getLevel("Level_0");
	const auto& layer = level.getLayer("Background");
	// get all the tiles in the Ground layer
	const auto& tiles_vector = layer.allTiles();

	Tilemap background = Tilemap(tiles_vector, 256, 256);

	const std::string vert_src = 
	#include "../src/shaders/sprite.vert"
	;

	const std::string frag_src =
	#include "../src/shaders/sprite.frag"
	;

	std::filesystem::path p = "./public/test/simplified/Level_0/_composite.png";
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

	//glUniform1i(glGetUniformLocation(ResourceManager::get_shader("sprite").id, "sprite"), 0); // GL_TEXTURE0
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
float rotation = 0;

void Game::Update(float dt)
{
	if (up) pos.y += speed * dt;
	if (down) pos.y -= speed * dt;
	if (left) pos.x -= speed * dt;
	if (right) pos.x += speed * dt;

	/*rotation += dt * 45.0f;
	if (rotation > 360.0f) rotation = 0.0f;*/
}

void Game::Render()
{
	renderer->draw_sprite(
		ResourceManager::get_texture("bor"), 
		pos,
		glm::vec2(1920.0f, 1080.0f),
		rotation,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
}