//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class WindowClass;
class ShaderControllerClass;
class TwoDGraphicsClass;
class ThreeDGraphicsClass;

//=================================
// included dependencies
#include <Windows.h>

class GraphicsClass{

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HINSTANCE, int);
	bool Render();
private:
	ShaderControllerClass* m_Shader;
	TwoDGraphicsClass* m_2DGraphics;
	ThreeDGraphicsClass* m_3DGraphics;
};