/* hello.c */
#include <stdio.h>

#if defined _WIN32
		// OpenGL on Windows needs Windows.h
#include <Windows.h>
#endif

#include <gl/GL.h>
#include <GLFW/glfw3.h>

int main(void) {
   puts("Hello, world!");

   GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

   return 0;
}
