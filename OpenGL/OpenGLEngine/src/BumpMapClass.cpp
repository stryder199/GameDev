#include "BumpMapClass.h"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <Windows.h>
#include <GL/glew.h>
#endif

BumpMapClass::BumpMapClass(void)
{
	mTextureID = 0;
	loaded = false;
}

BumpMapClass::~BumpMapClass(void)
{
}

bool BumpMapClass::Initialize(string filename, unsigned int textureUnit, bool wrap)
{
	bool result;

	result = LoadTextureFromFile(filename, textureUnit, wrap);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to Load Texture from a File", "Error", MB_OK );
#endif
		return false;
	}

	mTextureUnit = textureUnit;
	mWrap = wrap;

	return true;
}

void BumpMapClass::Shutdown()
{
	if(loaded)
	{
		glDeleteTextures(1, &mTextureID);
		loaded = false;
	}

	return;
}

bool BumpMapClass::LoadTextureFromFile(string filename, unsigned int textureUnit, bool wrap)
{
	bool result;
	int error;
	FILE* filePtr;
	unsigned int count;
	int width, height, bpp, imageSize;
	unsigned char* targaImage;
	TargaHeader targaFileHeader;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename.c_str(), "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if(bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if(!targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = fread(targaImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Set the unique texture unit in which to store the data.
	glActiveTexture(GL_TEXTURE0 + textureUnit);

	// Generate an ID for the texture.
	glGenTextures(1, &mTextureID);

	// Bind the texture as a 2D texture.
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Load the image data into the texture unit.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage);

	// Set the texture color to either wrap around or clamp to the edge.
	if(wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	// Set the texture filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate mipmaps for the texture.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Release the targa image data.
	delete [] targaImage;
	targaImage = 0;

	// Set that the texture is loaded.
	loaded = true;

	return true;
}

unsigned int BumpMapClass::getTextureUnit()
{
	return mTextureUnit;
}

bool BumpMapClass::getWrap()
{
	return mWrap;
}