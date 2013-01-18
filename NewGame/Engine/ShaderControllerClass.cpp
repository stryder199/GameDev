#include "ShaderControllerClass.h"
#include "PixelShaderClass.h"
#include "VertexShaderClass.h"

ShaderControllerClass::ShaderControllerClass()
{
}

ShaderControllerClass::~ShaderControllerClass()
{
}

bool ShaderControllerClass::Initialize()
{
}

bool ShaderControllerClass::Render()
{
	vertexFocus->Render();
	pixelFocus->Render();
}

void ShaderControllerClass::Set2DShaders()
{
	vertexFocus = TwoDVertexShader;
	pixelFocus = TwoDPixelShader;
}

void ShaderControllerClass::Set3DShaders()
{
	vertexFocus = ThreeDVertexShader;
	pixelFocus = ThreeDPixelShader;
}