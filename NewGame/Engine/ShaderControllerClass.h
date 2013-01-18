////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderControllerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERCONTROLLERCLASS_H_
#define _SHADERCONTROLLERCLASS_H_

/////////////////
// FORWARD DEC //
/////////////////
class VertexShaderClass;
class PixelShaderClass;

//////////////
// INCLUDES //
//////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderControllerClass
////////////////////////////////////////////////////////////////////////////////

class ShaderControllerClass{

public:
	ShaderControllerClass();
	~ShaderControllerClass();

	bool Initialize();
	bool Render();

	void Set3DShaders();
	void Set2DShaders();

private:
	VertexShaderClass* vertexFocus;
	PixelShaderClass* pixelFocus;

	VertexShaderClass* TwoDVertexShader;
	PixelShaderClass* TwoDPixelShader;

	VertexShaderClass* ThreeDVertexShader;
	PixelShaderClass* ThreeDPixelShader;

};