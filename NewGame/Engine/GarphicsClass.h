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
#include "Timer.h"

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
	Timer m_fpsTimer;
	int m_fps, m_fpsCount;
};