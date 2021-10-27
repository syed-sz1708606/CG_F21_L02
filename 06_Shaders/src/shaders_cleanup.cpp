//-----------------------------------------------------------------------------
// InitOpenGL_1.cpp //
//
// render 2 triangles to create a quad
// Add wireframe
// //+cleanup shaders
//-----------------------------------------------------------------------------
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "ShaderProgram.h"

// Global Variables
const char* APP_TITLE = "Computer Graphics - Render 2 triangles ";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gmainWindow;
bool gWireframe = false;

//Prototypes
void Print_OpenGL_Info();
void GLFW_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
bool initOpenGL();
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);


//Shaders



//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL()) {
		printf("Failed to initiliaze OpenGl");
		return EXIT_FAILURE;
	}

	//Triangle setup
	GLfloat vertices[] = {
		//first triangle           
		-0.5f, 0.5f, 0.0f,//top 			
		0.5f, 0.5f, 0.0f,//right
		0.5f, -0.5f, 0.0f,//left
		-0.5f, -0.5f, 0.0f,
	};

	GLint indicies[] = {
		0, 1, 2,
		0, 2, 3
	};

	//2 setup buffers on GPU
	GLuint vbo, vao, ibo; //ibo = element buffer or ibo(index buffer)

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //make it ask working buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); //make as active one
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0); //we can use 0 in the place of sizeof(GLfloat) * 3 because tightly packed (only position)
	glEnableVertexAttribArray(0);

	//setup index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("./shaders/basic.vert", "./shaders/basic.frag");



	// Rendering loop
	while (!glfwWindowShouldClose(gmainWindow))
	{
		//get and handle user input
		glfwPollEvents();

		//

		// Drawing
		glClear(GL_COLOR_BUFFER_BIT);


		shaderProgram.use();

		//create uniform variable
		GLfloat time = (GLfloat)glfwGetTime();
		GLfloat greenColor = (sin(time) / 2) + 0.5;

		shaderProgram.setUniform("vertColor", glm::vec4(0.0f, greenColor, 0.0f, 1.0f));

		//GLint vertexColor = glGetUniformLocation(shaderProgram.getProgram(), "vertColor");
		//glUniform4f(vertexColor, 0.0f, 1.0f, 0.0f, 1.0f);


		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // unbind

		glfwSwapBuffers(gmainWindow);
	}

	// Clean up

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}

//=====================================//
//Initiliation of OpenGl
bool initOpenGL()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	gmainWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gmainWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gmainWindow);

	// Initialize GLEW
	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	//Print OpenGl Info
	Print_OpenGL_Info();

	//set callbacks
	glfwSetKeyCallback(gmainWindow, GLFW_onKey);
	glfwSetFramebufferSizeCallback(gmainWindow, glfw_onFramebufferSize);

	return true;
}

//OpenGl information
void Print_OpenGL_Info()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Renderer               :%s\n", renderer);
	printf("GL Vendor               :%s\n", vendor);
	printf("GL Version               :%s\n", version);
	printf("GL Version               :%d.%d\n", major, minor);
	printf("GL GLSL Version               :%s\n", glslVersion);

}

void GLFW_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
}

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}