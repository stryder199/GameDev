#pragma once

//////////////////////
///Includes
#include <string>
#include <stdint.h>

//////////////////////
///Forward Dec
class ShaderClass;
class Matrix4x4;
class Vector3;
class Vector4;

using namespace std;

class ShaderProgram
{
public:
	ShaderProgram(void);
	~ShaderProgram(void);

	bool Initialize(string vertexShaderFilename, string fragShaderFilename);
	void Shutdown();

	bool SetShaderParameters(Matrix4x4 worldMatrix, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, 
							Vector3 lightDirection, Vector4 diffuseLightColor, Vector4 ambientLight, 
							Vector3 cameraPosition, Vector4 specularColor, float specularPower);

	ShaderClass* getVertexShader();
	ShaderClass* getFragmentShader();
	uint32_t getHandle();

	void activate();
	void deactivate();

private:
	bool LinkShaders();

	ShaderClass* mVertexShader;
	ShaderClass* mFragmentShader;

	uint32_t mHandle;
};
