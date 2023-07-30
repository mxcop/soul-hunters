#pragma once

#include <vector>
#include "shader.h"
#include <glm/glm.hpp>

class Light
{
    static Shader shader;

    /// Diameter of the light source.
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

public:
    Light() {};
    /// <param name="range">The diameter of the light.</param>
    Light(glm::vec2 pos, float range, float angle = 360.0f);

    static void setup();

    /// <summary>
    /// Compute the shadow mask.
    /// </summary>
    void compute(std::vector<glm::vec2>& shadow_edges);
    void draw();

    /// <summary>
    /// Set the projection matrix of the light source.
    /// </summary>
    void set_projection(glm::mat4 projection);

    float get_range() { return this->range; };

    glm::vec2 pos = { 0.0f, 0.0f };
    glm::vec2 dir = { 0.0f, 0.0f };

    //glm::vec4 color = { 1.0f, 1.0f, 0.7f, 0.8f };
    glm::vec4 color = {};
};