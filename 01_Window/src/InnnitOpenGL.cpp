//-----------------------------------------------------------------------------
// InitOpenGL_1.cpp //
//
// Creates a basic window and OpenGL 3.3 context using GLFW.
//-----------------------------------------------------------------------------
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// Global Variables
const char* APP_TITLE = "Computer Graphics - Hello Window";
const int gWindowWidth = 800;
const int gWindowHeight = 600;

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (pWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	// Initialize GLEW
	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Rendering loop
	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(pWindow);
	}

	// Clean up
	glfwTerminate();
	return 0;
}
