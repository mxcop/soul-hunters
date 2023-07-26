#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "game/game.h"
#include "engine/resource-manager.h"

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void joystick_callback(int jid, int event);

int joysticks[GLFW_JOYSTICK_LAST + 1];
int joystick_count = 0;

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1920;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 1080;

const float TARGET_FPS = 60.0f;

Game* game = nullptr;

typedef struct ScreenQuad {
    GLuint vao, vbo, ebo;
    Shader shader;
};

/// <summary>
/// Initialize the screen quad.
/// </summary>
ScreenQuad init_screen_quad() {
    ScreenQuad quad = {};

    // Init objects
    glGenVertexArrays(1, &quad.vao);
    glBindVertexArray(quad.vao);
    glGenBuffers(1, &quad.vbo);

    float vertices[] =
    {
        // x   y      u     v
        0.0f, 1.0f,  0.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.0f,  0.0f, 1.0f
    };

    // Upload the vertices to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Init more objects
    glGenBuffers(1, &quad.ebo);

    GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // Upload the indices (elements) to the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Pos atrribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    // Tex attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(GLfloat)));

    // Unbind the GL buffers:
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    const std::string vert_src =
    #include "./engine/shaders/sprite.vert"
    ;

    const std::string frag_src =
    #include "./engine/shaders/sprite.frag"
    ;

    // Load in shaders.
    quad.shader = ResourceManager::load_shader(vert_src.c_str(), frag_src.c_str(), nullptr, "screen_quad");

    // Set up shaders.
    quad.shader.use().set_int("sprite", 0); // GL_TEXTURE0

    glm::mat4 projection = glm::ortho(0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1000.0f);
    quad.shader.set_mat4("projection", projection);

    return quad;
}

typedef struct FrameBuffer {
    GLuint fbo, id;
};

/// <summary>
/// Initialize the frame buffer.
/// </summary>
FrameBuffer init_framebuffer() {
    FrameBuffer buffer = {};
    glGenFramebuffers(1, &buffer.fbo);
    glGenTextures(1, &buffer.id);
    glBindTexture(GL_TEXTURE_2D, buffer.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 320, 180, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.id, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return buffer;
}

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetJoystickCallback(joystick_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ScreenQuad screen_quad = init_screen_quad();
    FrameBuffer frame_buffer = init_framebuffer();

    game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

    // initialize game
    // ---------------
    game->Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float frameCounter = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // update game state
        // -----------------
        game->Update(deltaTime);

        if (frameCounter >= (1.0 / TARGET_FPS)) 
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer.fbo);
            frameCounter = 0;

            // render
            // ------
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            game->Render();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            // Bind the GL buffers:
            glBindVertexArray(screen_quad.vao);
            glBindBuffer(GL_ARRAY_BUFFER, screen_quad.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad.ebo);

            // Compute the model matrix:
            glm::mat4 model = glm::mat4(1.0f/* Identity matrix */);
            model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));

            // Set the uniforms within the shader:
            screen_quad.shader.use();
            screen_quad.shader.set_mat4("model", model);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, frame_buffer.id);

            // Draw the quad with texture.
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Unbind the GL buffers:
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            // glBlitFramebuffer(0, 0, 320, 180, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
            //     GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            glfwSwapBuffers(window);
        }

        frameCounter += deltaTime;
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::clear();

    glfwTerminate();
    return 0;
}

void joystick_callback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
        joysticks[joystick_count++] = jid;
    else if (event == GLFW_DISCONNECTED)
    {
        int i;

        for (i = 0; i < joystick_count; i++)
        {
            if (joysticks[i] == jid)
                break;
        }

        for (i = i + 1; i < joystick_count; i++)
            joysticks[i - 1] = joysticks[i];

        joystick_count--;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    game->ProcessKeyInput(key, action);

    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            game->keys[key] = true;
        else if (action == GLFW_RELEASE)
            game->keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}