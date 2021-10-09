//-----------------------------------------------------------------------------
// 
// - Two triangle 
// - add wireframe  
// - add indices for optimisation 
//	 modify the shaders to read the color from the data
//  + clean up the shaders: make the code more readable 
//-----------------------------------------------------------------------------
#include <iostream>
using namespace std;

#define GLEW_STATIC //!! make it static because 

#include "GL/glew.h" // must include this glew before glfw 
#include "GLFW/glfw3.h"

#include "ShaderProgram.h"

//image loader 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

// Global Variables
const char* APP_TITLE = "Computer Graphics - Textures   ";

//window dimensions 
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gmainWindow;			// the main windown is created here as a global variable 
bool gWireframe = false;


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



	//1. set up triangle verticies array  ( vertex input ) ===================== Vertex Specification 
	// use indices for optimisation

	GLfloat vertices[] = {

		//positions				//text cords


		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	   //top left
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,    //top right 
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,	   //bottom right 
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,	   //bottom left 

	};

	GLuint indices[] = { // for first triangle and second 

		0, 1, 2,		//first 
		0, 2, 3 	    //second  

	};



	// setup buffers on the GPU 
	//==== VAO defines what data a vertex has ( pos, color , texture , normals etc) 
	//==== VBO defines the data itself 
	//==== Attribute pointers define where and how shaders can access vertex data 

	GLuint vbo, vao, ibo; // element buffer is ibo 

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);																//make as active vertex array 

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);													//make as workig buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);						//give attrib position 
	glEnableVertexAttribArray(0);		//IMPORTANT 

	//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);		//IMPORTANT 

	//setup index buffer 
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/basic.vert", "res/shaders/basic.frag");


	//Add textures ***************
	//****************************
	int width, height, nrComponent;

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set texture parameters wrap  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// set texture parameters filter  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load and create the texture


	unsigned char* texData = stbi_load("res/images/Mario1.png", &width, &height, &nrComponent, 0);
	if (!texData)
	{
		printf("Failed to load image file: %s", "res/images/Mario2.jpeg");
	}

	GLenum format = 0;
	if (nrComponent == 0)
		format = GL_RED;
	if (nrComponent == 3)
		format = GL_RGB;
	if (nrComponent == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
	//glGenerateMipmap(gl_texture_2D);

	stbi_image_free(texData);

	// set the texture and rendering 
	shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler1"), 0);

	// #################Rendering loop (loop until window is closed) ##############-- > game loop 
	while (!glfwWindowShouldClose(gmainWindow))
	{
		// Get + handle user inputs 
		glfwPollEvents();


		//====Drawing=====//

		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		// create uniform variables 

		GLfloat time = (GLfloat)glfwGetTime();
		GLfloat greenColor = (sin(time) / 2) + 0.5;

		shaderProgram.setUniform("vertColor", glm::vec4(0.0f, greenColor, 0.0f, 1.0f));

		//GLint vertexColor = glGetUniformLocation(shaderProgram.getProgram(), "vertColor");
		//glUniform4f(vertexColor, 0.0f, greenColor, 0.0f, 1.0f);

		glBindVertexArray(vao);
		// glDrawArrays(GL_TRIANGLES, 0, 6); ....... not the way to draw now 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//swap the screen buffers --> the concept of double buffers 
		glfwSwapBuffers(gmainWindow);
	}

	// Clean up

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}


//===================================================
//Initialisaiton of OpenGL 
bool initOpenGL()

{

	//initialize glfw 
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		cerr << "GLFW initialization failed" << std::endl;
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
	glClearColor(0.1f, 0.0f, 0.5f, 1.0f);

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
// handle keyboard
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

}

void glfw_onFrameBufferSize(GLFWwindow* window, int  width, int height)
{
	glViewport(0, 0, width, height);
}


