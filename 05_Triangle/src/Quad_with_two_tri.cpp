//-----------------------------------------------------------------------------
// InitOpenGL_1.cpp //
//
// render 2 triangles to create a quad
// Add wireframe
//-----------------------------------------------------------------------------
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 fragColor;"
"void main()"
"{"
"		fragColor = vec4(0.85f,0.95f,0.3f,1.0);"
"}";

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

	//3 Vetex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	//check for compile error in shader code
	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		printf("Error: %s Vertex shader failed to compile.", infoLog);
	}

	//4. create a fragment shader
	GLchar infoLogfs[512];
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);

	//check for compile error in fragment code
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fs, sizeof(infoLogfs), NULL, infoLogfs);
		printf("Error: %s Fragment shader failed to compile.", infoLogfs);
	}

	//5. create shader program and link our vertex and fragment shader
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error: Shader program failed to link." << infoLog << std::endl;
	}

	//clean up shaders
	glDeleteShader(vs);
	glDeleteShader(fs);


	// Rendering loop
	while (!glfwWindowShouldClose(gmainWindow))
	{
		//get and handle user input
		glfwPollEvents();

		//

		// Drawing
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // unbind

		glfwSwapBuffers(gmainWindow);
	}

	// Clean up
	glDeleteProgram(shaderProgram);
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