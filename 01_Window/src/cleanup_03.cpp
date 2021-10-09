//-----------------------------------------------------------------------------
// InitOpenGL_1.cpp //
// 2. print graphic card info and shader 
//
// Creates a basic window and OpenGL 3.3 context using GLFW.
//-----------------------------------------------------------------------------
#include <iostream>

#define GLEW_STATIC //!! make it static because 
#include "GL/glew.h" // must include this glew before glfw 
#include "GLFW/glfw3.h"

// Global Variables
const char* APP_TITLE = "Computer Graphics - Cleanup Window ";

//window dimensions 
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gmainWindow;


//add prototype
void print_OpenGL_info();
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFrameBufferSize(GLFWwindow* window, int  width, int height);
bool initOpenGL();

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main() {

	if (!initOpenGL()) {
		printf("Failed to initialize OpenGL");
		return EXIT_FAILURE;

	}


	// #################Rendering loop (loop until window is closed) ##############-- > game loop 
	while (!glfwWindowShouldClose(gmainWindow))
	{
		// Get + handle user inputs 
		glfwPollEvents();


		//====Drawing=====//

		glClear(GL_COLOR_BUFFER_BIT);

		//swap the screen buffers --> the concept of double buffers 
		glfwSwapBuffers(gmainWindow);
	}

	// Clean up
	glfwTerminate();
	return 0;
}


//===================================================
//Initialisaiton of OpenGL 
bool initOpenGL()

{

	//initialize glfw 
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		//std::cerr << "GLFW initialization failed" << std::endl;
		return EXIT_FAILURE;
	}

	//setup glfw properties and set the opengl version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //3.3 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//no backwards compatibility 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//allow forward compatibility 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);



	//create main window 
	gmainWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gmainWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		//terminate window 
		glfwTerminate();
		return -1;
	}

	//set context for glew to use 

	glfwMakeContextCurrent(gmainWindow);

	// Initialize GLEW
	//glewExperimental = GL_TRUE;


	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//====clear the color buffer =====//
	glClearColor(0.2f, 0.9f, 0.6f, 1.0f);

	//print info 
	print_OpenGL_info();

	//set callbacks 
	glfwSetKeyCallback(gmainWindow, glfw_onKey);

	glfwSetFramebufferSizeCallback(gmainWindow, glfw_onFrameBufferSize);


	return true;
}




// print the openGL vrsion information 
void print_OpenGL_info()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Renderer        :%s\n", renderer);
	printf("GL Vendor        :%s\n", vendor);
	printf("GL Version        :%s\n", version);
	printf("GL glslVersion    :%s\n", glslVersion);
	printf("GL Version (int)    :%d.%d\n", major, minor);



}

//create key callback 
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void glfw_onFrameBufferSize(GLFWwindow* window, int  width, int height)
{
	glViewport(0, 0, width, height);
}