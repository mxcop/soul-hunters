#pragma once
#include "shader.h"
#include <glm/glm.hpp>

class Light
{
public:
    Light(glm::vec2 pos, float range);

    static void setup();

    void compute();
    void draw();

private:
    static Shader shader;

    glm::vec2 pos = { 0.0f, 0.0f };
    float range = 5.0f;
};