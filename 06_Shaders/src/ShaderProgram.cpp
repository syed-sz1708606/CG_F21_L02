#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram()
	:mHandle(0)
{}

ShaderProgram::~ShaderProgram() {
	//delete the program
	glDeleteProgram(mHandle);
}

bool ShaderProgram::loadShaders(const char* vsfilename, const char* fsfilename) {
	string vsString = fileToString(vsfilename);
	string fsString = fileToString(fsfilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);
	glCompileShader(fs);

	checkCompileErrors(vs, VERTEX);


	checkCompileErrors(fs, FRAGMENT);

	//check for compile error in shader code


	//4. create a fragment shader



	//check for compile error in fragment code


	//5. create shader program and link our vertex and fragment shader
	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompileErrors(mHandle, PROGRAM);



	//clean up shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

void ShaderProgram::use() {
	if (mHandle > 0)
		glUseProgram(mHandle);
}

GLuint ShaderProgram::getProgram() const {
	return mHandle;
}

string ShaderProgram::fileToString(const string& filename) {
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);
		if (!file.fail())
		{
			ss << file.rdbuf();
		}
		file.close();
	}
	catch (const std::exception&)
	{
		printf("Error in reading the shader filename!");
	}

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type) {

	GLint status;

	if (type == PROGRAM) {
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, sizeof(length), &length, &errorLog[0]);
			std::cerr << "Error: Shader program failed to link." << errorLog << std::endl;
		}
	}
	else // vertex or fragment shader
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, sizeof(length), &length, &errorLog[0]);
			std::cerr << "Error: Fragment Shader failed to compile." << errorLog << std::endl;
		}
	}

}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);

	//what if we couldnt find the location
	if (it == mUniformLocations.end()) {
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	//if we found the name
	return mUniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) {

	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) {

	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}