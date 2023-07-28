#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "../engine/renderer/texture.h"
#include "../engine/renderer/sprite-renderer.h"
#include "../engine/data/lake.h"

class Ghost {
public:
    static Lake<Ghost> ghosts;

    static void make(glm::vec2 pos, float hp) { 
        size_t id = ghosts.add(Ghost(pos, hp, 0));
        ghosts.get(id).id = id;
    };

    void update(glm::vec2 player1, glm::vec2 player2, float dt);
    void draw(SpriteRenderer& renderer) const;

    glm::vec2 get_pos() { return this->pos; };

    float hp = 5.0f;

private:
    Ghost(glm::vec2 pos, float hp, int id);

    float speed = 3.0f;
    int id;

    glm::vec2 pos = {};
    float time = 0.0f;
    bool move_right;
};