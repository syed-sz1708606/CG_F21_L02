
/////////////////////////
/////////////////////////
///////////////////////
///////////////////////
////////////////////////

//-----------------------------------------------------------------------------
// 
// - Two triangle 
// - add wireframe  
// - add indices for optimisation 
//	 modify the shaders to read the color from the data
//  + clean up the shaders: make the code more readable 
// add transformation including translation, rotation , scaling
//-----------------------------------------------------------------------------
#include <iostream>
using namespace std;

#define GLEW_STATIC //!! make it static because 

#include "GL/glew.h" // must include this glew before glfw - interface with modern openGL 
#include "GLFW/glfw3.h" //creeates windows and openGL context, allows user input 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
//image loader 

// Global Variables
const char* APP_TITLE = "Computer Graphics - Projection   ";
const std::string texture1fileName = "res/images/brick1.jpg";
const std::string texture2fileName = "res/images/SuperM.png";

const std::string gridImage = "res/images/brick1.jpg";


//window dimensions 
int gWindowWidth = 800;
int gWindowHeight = 600;
GLFWwindow* gmainWindow;			// the main windown is created here as a global variable 
bool gWireframe = false;


//Experiment eith translation 
bool transDirection = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float increment = 0.01f;

//Experiment with Rotation
float curAngle = 0.0f;


//Experiment with Scaling 
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.2f;

//setting fps camera 
FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f)); 
const double ZOOM_SENSITIVITY = -3.0f; 
const float MOVE_SPEED = 5.0f; 
const float MOUSE_SENSITIVITY = 0.1f; 




//add prototype
void print_OpenGL_info();
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFrameBufferSize(GLFWwindow* window, int  width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY); 
void update(double elapsedTime); 

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

		// position         // tex coords



	   // front face

	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,

	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

	   -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,

		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,



		// back face

		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,

		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,



		 // left face

		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		 -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,

		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,



		 // right face

		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,

		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

		  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,

		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,



		  // top face

		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,

		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,

		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,



		  // bottom face

		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,

		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,

		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,

		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

	};


	glm::vec3 cubePos(0.0f, 0.0f, 0.0f);
	glm::vec3 floorPos(0.0f, 1.0f, 0.0f); 

	// setup buffers on the GPU 
	//==== VAO defines what data a vertex has ( pos, color , texture , normals etc) 
	//==== VBO defines the data itself 
	//==== Attribute pointers define where and how shaders can access vertex data 

	GLuint vbo, vao; // element buffer is ibo 

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);																//make as active vertex array 

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);													//make as workig buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	////////////////////////
	//Positions/////////////
	////////////////////////
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);						//give attrib position 
	glEnableVertexAttribArray(0);		//IMPORTANT 

	///////////////////////
	//texture//////////////
	///////////////////////
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);		//IMPORTANT 

	//setup index buffer 
	/*glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), indices, GL_STATIC_DRAW);*/



	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/projection.vert", "res/shaders/projection.frag");

	Texture texture1;
	texture1.loadTexture(texture1fileName, true);

	Texture texture2;
	texture2.loadTexture(texture2fileName, true);

	Texture floorTexture;
	floorTexture.loadTexture(gridImage, true);

	double lastTime = glfwGetTime();
	float cubeAngle = 0.0f;


	// #################Rendering loop (loop until window is closed) ##############-- > game loop 
	while (!glfwWindowShouldClose(gmainWindow))
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;


		// Get + handle user inputs 
		glfwPollEvents();
		update(deltaTime); 

		//====Drawing=====//

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		texture1.bind(0);
		texture2.bind(1);

		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler1"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler2"), 1);

		shaderProgram.use();

		// create uniform variables 

		//GLfloat time = (GLfloat)glfwGetTime();
		//GLfloat greenColor = (sin(time) / 2) + 0.5;

		//shaderProgram.setUniform("vertColor", glm::vec4(0.0f, greenColor, 0.0f, 1.0f));

		//GLint vertexColor = glGetUniformLocation(shaderProgram.getProgram(), "vertColor");
		//glUniform4f(vertexColor, 0.0f, greenColor, 0.0f, 1.0f);

		glfwSwapInterval(1); // min number of screeen updates to wait for until buffers swapped by 

		//////////////////////////
		//Transformations//////
		//////////////////////////



		if (transDirection)
			offset += increment;
		else
			offset -= increment;
		if (abs(offset) >= maxOffset)
			transDirection = !transDirection;

		////////////////////////////

		curAngle += 0.5f;
		if (curAngle >= 360.0f)
			curAngle -= 360.0f;

		///////////////////////////

		if (sizeDirection)
			curSize += 0.01f;
		else
			curSize -= 0.01f;
		if (curSize >= maxSize || curSize <= minSize)
			sizeDirection = !sizeDirection;

		//model , view , projection. 
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view(1.0), projection(1.0);
		/////////////////////////////


		//glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		//glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		//glm::vec3 up(0.0f, 1.0f, 0.0f);

		// create model matirx 
		/*cubeAngle += (float)deltaTime * 50.0f;
		if (cubeAngle >= 360.0f)
			cubeAngle = 0.0f;*/

		//view = fpsCamera.getViewMatrix(); 

		/*model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));*/

		//creare the view matrix 
		view = fpsCamera.getViewMatrix();

		//view = glm::lookAt(camPos, camPos + targetPos, up);

		//create projection matrix 
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);



		//transform = glm::translate(transform, glm::vec3(offset, 0.0f, 0.0f));
		//transform = glm::rotate(transform, glm::radians(curAngle), glm::vec3(1.0f, 1.0f, 1.0f));
		//transform = glm::scale(transform, glm::vec3(curSize, 0.4f, curSize));

		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		glBindVertexArray(vao);
		// glDrawArrays(GL_TRIANGLES, 0, 6); ....... not the way to draw now 
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//draw floor 
		floorTexture.bind(0); 
		model = (glm::translate(model, floorPos) * glm::scale(model,glm::vec3(10.0f, 0.01f, 10.0f)));
		shaderProgram.setUniform("model", model); 
		glBindVertexArray(vao); 
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//swap the screen buffers --> the concept of double buffers 
		lastTime = currentTime;
		glfwSwapBuffers(gmainWindow);
	}

	// Clean up

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ibo); 
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

	//print info of graphics card 
	print_OpenGL_info();

	//set callbacks 
	glfwSetKeyCallback(gmainWindow, glfw_onKey);

	glfwSetFramebufferSizeCallback(gmainWindow, glfw_onFrameBufferSize);

	glfwSetScrollCallback(gmainWindow, glfw_onMouseScroll);

	//set mouse to center of screen 
	glfwSetInputMode(gmainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gmainWindow, gWindowWidth / 2.0, gWindowHeight / 2.0); 

	//enable alpha support 
	glEnable(GL_BLEND);


	//enable depth dest 
	glEnable(GL_DEPTH_TEST);

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

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
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
	gWindowWidth = width;
	gWindowHeight = height;
	glViewport(0, 0, width, height);
}


//** handle mouse scrolling ** // 

void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY; 
	fov = glm::clamp(fov, 1.0, 120.0); 
	fpsCamera.setFOV((float)fov); 
}

//update the camera every frame 
//*********************************************
void update(double elapsedTime)
{
	double mouseX, mouseY; 
	glfwGetCursorPos(gmainWindow, &mouseX, &mouseY); 

	//rotate the camera 
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY); 

	glfwSetCursorPos(gmainWindow, gWindowWidth / 2.0, gWindowHeight / 2.0); 

	//handle the camera movement based on the WASD keys 
	//for forward and back 
	if (glfwGetKey(gmainWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float) elapsedTime * fpsCamera.getlook()); 
	if (glfwGetKey(gmainWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getlook());

	//left and right 
	if (glfwGetKey(gmainWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
	if (glfwGetKey(gmainWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());

	// up and down x and z 


	if (glfwGetKey(gmainWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	if (glfwGetKey(gmainWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());


 }