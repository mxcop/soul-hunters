#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "../engine/renderer/texture.h"
#include "../engine/renderer/sprite-renderer.h"

class Ghost {
    float speed = 3.0f;

public:
    static void reserve_ghosts(size_t num) { ghosts.reserve(num); };
	static bool make(glm::vec2 pos, float hp);
    static void update_all(glm::vec2 player1, glm::vec2 player2, float dt);
    static void draw_all(SpriteRenderer& renderer);
    static std::vector<Ghost>& get_ghosts() { return ghosts; };
    static bool deactivate(int id);

private:
    Ghost(glm::vec2 pos, float hp, int id);

    static std::vector<Ghost> ghosts;

    int id;
    bool is_active = true;

    glm::vec2 pos = {};
    float hp = 5.0f;
    float time = 0.0f;
    bool move_right;

    void update(glm::vec2 player1, glm::vec2 player2, float dt);
    void draw(SpriteRenderer& renderer) const;
};