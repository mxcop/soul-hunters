#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

    // initialize game
    // ---------------
    game->init();

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
        game->update(deltaTime);

        if (frameCounter >= (1.0 / TARGET_FPS)) 
        {
            frameCounter = 0;

            // fixed update
            // ------------
            game->fixed_update();

            // render
            // ------
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            game->render();

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
    game->key_input(key, action);

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