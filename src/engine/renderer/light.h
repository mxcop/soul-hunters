#pragma once

#include <vector>
#include "shader.h"
#include <glm/glm.hpp>

class Light
{
public:
    Light() {};
    Light(glm::vec2 pos, float range, float angle = 360.0f);

    static void setup();

    void compute(std::vector<glm::vec2>& shadow_edges);
    void draw();

    /// <summary>
    /// Set the projection matrix of the light source.
    /// </summary>
    void set_projection(glm::mat4 projection);

    float get_range() { return this->range; };

    glm::vec2 pos = { 0.0f, 0.0f };
    glm::vec2 dir = { 0.0f, 0.0f };

private:
    static Shader shader;
    float range = 5.0f;
    float angle = 360.0f;
    int edges = 0;

    struct {
        GLuint vao = 0;
        GLuint vbo = 0;
    } shadow;

    struct {
        GLuint vao = 0;
        GLuint vbo = 0;
    } light;

    void gl_init();
};