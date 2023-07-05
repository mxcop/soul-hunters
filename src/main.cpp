///* hello.c */
//#if defined _WIN32
//		// OpenGL on Windows needs Windows.h
//#include <Windows.h>
//#endif
//
//#include <glad/glad.h>
//#include <GL/gl.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/ext/matrix_clip_space.hpp>
//
//#include "linmath.h"
//
//#include <stdlib.h>
//#include <stdio.h>
//
//static const struct
//{
//	float x, y;
//	float r, g, b;
//} vertices[3] =
//{
//	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
//	{  0.6f, -0.4f, 0.f, 1.f, 0.f },
//	{   0.f,  0.6f, 0.f, 0.f, 1.f }
//};
//
//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vCol;\n"
//"attribute vec2 vPos;\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
//"    color = vCol;\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";
//
//void error_callback(int error, const char* description)
//{
//	fprintf(stderr, "Error: %s\n", description);
//}
//
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//	}
//}
//
//int main(void) {
//	puts("Hello, world!");
//
//	GLFWwindow* window;
//	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
//	GLint mvp_location, vpos_location, vcol_location;
//
//	glfwSetErrorCallback(error_callback);
//
//
//   if (!glfwInit())
//   {
//	   // Initialization Failed
//	   exit(EXIT_FAILURE);
//   }
//
//   window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
//   if (!window)
//   {
//	   // Window or OpenGL Context Creation Failed
//	   glfwTerminate();
//	   exit(EXIT_FAILURE);
//	   
//   }
//
//   glfwSetKeyCallback(window, key_callback);
//
//   glfwMakeContextCurrent(window); 
//   gladLoadGL();
//   glfwSwapInterval(1);
//   glfwPollEvents();
//
//   glGenBuffers(1, &vertex_buffer);
//   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//   glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//   glCompileShader(vertex_shader);
//
//   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//   glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//   glCompileShader(fragment_shader);
//
//   program = glCreateProgram();
//   glAttachShader(program, vertex_shader);
//   glAttachShader(program, fragment_shader);
//   glLinkProgram(program);
//
//   mvp_location = glGetUniformLocation(program, "MVP");
//   vpos_location = glGetAttribLocation(program, "vPos");
//   vcol_location = glGetAttribLocation(program, "vCol");
//
//   glEnableVertexAttribArray(vpos_location);
//   glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
//	   sizeof(vertices[0]), (void*)0);
//   glEnableVertexAttribArray(vcol_location);
//   glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
//	   sizeof(vertices[0]), (void*)(sizeof(float) * 2));
//
//   while (!glfwWindowShouldClose(window))
//   {
//	   // Keep Running
//	   float ratio;
//	   int height, width;
//	   mat4x4 m, p, mvp;
//
//	   glfwGetFramebufferSize(window, &width, &height);
//	   ratio = width / (float)height;
//
//	   glViewport(0, 0, width, height);
//	   glClear(GL_COLOR_BUFFER_BIT);
//
//	   mat4x4_identity(m);
//	   mat4x4_rotate_Z(m, m, (float)glfwGetTime());
//	   mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//	   mat4x4_mul(mvp, p, m);
//
//	   glUseProgram(program);
//	   glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
//	   glDrawArrays(GL_TRIANGLES, 0, 3);
//
//	   glfwSwapBuffers(window);
//	   glfwPollEvents();
//   }
//
//   glfwDestroyWindow(window);
//   glfwTerminate();
//   exit(EXIT_SUCCESS);
//
//   return 0;
//}

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/game.h"
#include "renderer/resource-manager.h"

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    Breakout.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        Breakout.ProcessInput(deltaTime);

        // update game state
        // -----------------
        Breakout.Update(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.keys[key] = true;
        else if (action == GLFW_RELEASE)
            Breakout.keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
