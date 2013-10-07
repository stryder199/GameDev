#pragma once

//////////////////////
///Includes
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

//////////////////////
///Forward Dec

using namespace std;

class ShaderClass
{
public:

	typedef enum
	{
		VERTEX		= 0x8B31,	/**< a vertex shader. */
		GEOMETRY	= 0x8DD9,	/**< a geometry shader. */
		FRAGMENT	= 0x8B30	/**< a fragment shader. */
	} ShaderType;

	ShaderClass(void);
	~ShaderClass(void);

	bool Initialize(string filename, ShaderType shaderType);
	void Shutdown();

	uint32_t getHandle();
	ShaderType getType();

private:
	bool LoadShaderFromFile(string filename);
	bool InitializeOnGPU();

	string mShaderFileContents;
	uint32_t mHandle;
	ShaderType mType;
};
