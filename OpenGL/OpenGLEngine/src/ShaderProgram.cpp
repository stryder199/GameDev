#include "ShaderProgram.h"
#include "ShaderClass.h"
#include "openglerror.h"
#include "MatrixClass.h"
#include "VectorClass.h"

#ifdef _WIN32
#include <Windows.h>
#endif

ShaderProgram::ShaderProgram(void)
{
	mFragmentShader = 0;
	mVertexShader = 0;
	mHandle = 0;
}

ShaderProgram::~ShaderProgram(void)
{
}

bool ShaderProgram::Initialize(string vertexShaderFilename, string fragShaderFilename)
{
	bool result;

	mVertexShader = new ShaderClass();
	if(!mVertexShader)
	{
		return false;
	}

	result = mVertexShader->Initialize(vertexShaderFilename, ShaderClass::VERTEX);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Vertex Shader Init Failure", "Error", MB_OK );
#endif
		return false;
	}

	mFragmentShader = new ShaderClass();
	if(!mFragmentShader)
	{
		return false;
	}

	result = mFragmentShader->Initialize(fragShaderFilename, ShaderClass::FRAGMENT);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Fragment Shader Init Failure", "Error", MB_OK );
#endif
		return false;
	}

	result = LinkShaders();
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to Link Shaders", "Error", MB_OK );
#endif
		return false;
	}

	return true;
}

bool ShaderProgram::SetShaderParameters(Matrix4x4 worldMatrix, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix,
										Vector3 lightDirection, Vector4 diffuseLightColor, Vector4 ambientLight,
										Vector3 cameraPosition, Vector4 specularColor, float specularPower)
{
	unsigned int location;

	// Set the world matrix in the vertex shader.
	location = glGetUniformLocation(mHandle, "worldMatrix");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find worldMatrix Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniformMatrix4fv(location, 1, false, worldMatrix.getElements());

	// Set the view matrix in the vertex shader.
	location = glGetUniformLocation(mHandle, "viewMatrix");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find viewMatrix Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniformMatrix4fv(location, 1, false, viewMatrix.getElements());

	// Set the projection matrix in the vertex shader.
	location = glGetUniformLocation(mHandle, "projectionMatrix");
	if(location == -1)
	{

#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find projectionMatrix Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniformMatrix4fv(location, 1, false, projectionMatrix.getElements());

	//Set the camera position in the vertex shader
	location = glGetUniformLocation(mHandle, "cameraPosition");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find cameraPosition Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniform3fv(location, 1, cameraPosition.getElements());

	//Set the light direction in the fragment shader
	location = glGetUniformLocation(mHandle, "lightDirection");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find lightDirection Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniform3fv(location, 1, lightDirection.getElements());

	//Set the diffuse light color in the fragment shader
	location = glGetUniformLocation(mHandle, "diffuseLightColor");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find diffuseLightColor Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniform4fv(location, 1, diffuseLightColor.getElements());

	//Set the ambient light in the fragment shader
	location = glGetUniformLocation(mHandle, "ambientLightColor");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find ambientLight Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniform4fv(location, 1, ambientLight.getElements());

	//Set the specular light in the fragment shader
	location = glGetUniformLocation(mHandle, "specularPower");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find specularPower Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniform1f(location, specularPower);

	//Set the specular light color in the fragment shader
	location = glGetUniformLocation(mHandle, "specularLightColor");
	if(location == -1)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to find specularColor Uniform", "Error", MB_OK );
#endif
		return false;
	}
	glUniform4fv(location, 1, specularColor.getElements());

	return true;
}

bool ShaderProgram::LinkShaders()
{
	if(mHandle > 0)
		return true;

	mHandle = glCreateProgram();
	GLenum glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error creating shader program " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}

	glAttachShader(mHandle, mFragmentShader->getHandle());
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error creating program " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}

	glAttachShader(mHandle, mVertexShader->getHandle());
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error attaching vertex shader " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}

	glBindAttribLocation(mHandle, 0, "inputVertex");
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error Finding Input Param " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}

	glBindAttribLocation(mHandle, 1, "inputNormal");
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error Finding Input Param " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}

	glBindAttribLocation(mHandle, 2, "inputColor");
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error Finding Input Param " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}


	glLinkProgram(mHandle);
	glError = glGetError();

	if (glError != GL_NO_ERROR)
	{
#ifdef _WIN32
		string error = "Error Linking Program " + getErrorString(glError);
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
	}

	int result = 1;
	glGetProgramiv(mHandle, GL_LINK_STATUS, & result);

	if (!result)
	{
		int length = 0;

		glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, & length);

		char * info = new char[length];
		int written = 0;

		glGetProgramInfoLog(mHandle, length, &written, info);
		string strInfo(info);
		delete[] info;

		cerr << "Error linking program" << endl;
		cerr << strInfo.c_str() << endl;
	}

	return (bool)result;
}

void ShaderProgram::Shutdown()
{
}

void ShaderProgram::activate()
{
	glUseProgram(mHandle);
}

void ShaderProgram::deactivate()
{
	glUseProgram(0);
}

ShaderClass* ShaderProgram::getVertexShader()
{
	return mVertexShader;
}

ShaderClass* ShaderProgram::getFragmentShader()
{
	return mFragmentShader;
}

uint32_t ShaderProgram::getHandle()
{
	return mHandle;
}