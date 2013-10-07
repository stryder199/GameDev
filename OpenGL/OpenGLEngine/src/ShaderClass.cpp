#include "ShaderClass.h"
#include "openglerror.h"

ShaderClass::ShaderClass(void)
{
	mHandle = 0;
}

ShaderClass::~ShaderClass(void)
{
}

bool ShaderClass::Initialize(string filename, ShaderType shaderType)
{
	bool result;

	mType = shaderType;

	result = LoadShaderFromFile(filename);
	if(!result)
		return false;

	result = InitializeOnGPU();
	if(!result)
		return false;

	return true;
}

void Shutdown()
{
}

bool ShaderClass::LoadShaderFromFile(string filename)
{
	ifstream fin;

	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	mShaderFileContents.assign( (std::istreambuf_iterator<char>(fin) ),
		(std::istreambuf_iterator<char>()    ) );

	mShaderFileContents = mShaderFileContents + '\0';

	return true;
}

bool ShaderClass::InitializeOnGPU()
{
	if(mHandle > 0)
		return true;

	mHandle = glCreateShader((uint32_t)mType);
	GLenum glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
		cerr << "Error creating shader " << getErrorString(glError).c_str() << endl;
	}

	const char *strings[] = { mShaderFileContents.c_str() };

	glShaderSource(mHandle, 1, strings, NULL);
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
		cerr << "Error loading shader " << getErrorString(glError).c_str() << endl;
	}

	glCompileShader(mHandle);
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
		cerr << "Error compiling shader " << getErrorString(glError).c_str() << endl;
	}

	int result = 1;
	glGetShaderiv(mHandle, GL_COMPILE_STATUS, & result);

	if (!result)
	{
		int length = 0;

		glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, & length);

		char * info = new char[length];
		int written = 0;

		glGetShaderInfoLog(mHandle, length, &written, info);
		std::string strInfo(info);
		delete[] info;

		cerr << "Error compiling shader " << endl;
		cerr << strInfo.c_str() << endl;
	}

	return (bool)result;
}

uint32_t ShaderClass::getHandle()
{
	return mHandle;
}

ShaderClass::ShaderType ShaderClass::getType()
{
	return mType;
}