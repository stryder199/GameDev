//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class WindowClass;
class D3DClass;
class CameraClass;
class ShaderControllerClass;
class TwoDGraphicsClass;
class ThreeDGraphicsClass;
class PlayerClass;

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

	CameraClass* getCamera();
	PlayerClass* getPlayer();

private:
	CameraClass* m_Camera;
	ShaderControllerClass* m_Shader;
	TwoDGraphicsClass* m_2DGraphics;
	ThreeDGraphicsClass* m_3DGraphics;
};