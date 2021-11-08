#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION // becasue it is a header file type library
#include "stb_image/stb_image.h"


Texture::Texture() :mTexture(0) {

}

Texture::~Texture() {
	glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(const string& fileName, bool generateMipMaps)
{

	int width, height, nrcomponent;
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	//set texture parameters filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//set texture parameters wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load and create texture
	unsigned char* texelData = stbi_load(fileName.c_str(), &width, &height, &nrcomponent, 0);
	if (!texelData)
	{
		printf("Failing to load iamge file %s", fileName.c_str());
	}

	GLenum format = 0;
	if (nrcomponent == 0)
		format = GL_RED;
	if (nrcomponent == 3)
		format = GL_RGB;
	if (nrcomponent == 4)
		format = GL_RGBA;


	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texelData);
	//glGenerateMipmap(GL_TEXTURE_2D);

	// first First Argument: Texture Target (what we have bound our texture to)
	// Second Argument: Mipmap Level (see next slide)
	// Third Argument: Format of the stored data. RGB is just Red, Green Blue values. Also RGBA which has an Alpha Channel(transparency).Several others, not important here
	// Fourth and Fifth Argument: Width and height of the texture. 
	// This should always be 0.
	//  Format of the data being loaded (as opposed to stored 
	//  The data type of the values (int, float, byte, etc
	// Ninth Argument: The data itself.

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texelData);
	glBindTexture(GL_TEXTURE_2D, 0); //unbinding

	

	return true;
}

void Texture::bind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit); 
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind 
}