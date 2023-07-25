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

    void set_pos(glm::vec2 pos);

    /// <summary>
    /// Set the projection matrix of the light source.
    /// </summary>
    void set_projection(glm::mat4 projection);

private:
    static Shader shader;

    glm::vec2 pos = { 0.0f, 0.0f };
    float range = 5.0f;

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