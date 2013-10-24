//=================================
// include guard
#ifndef GraphicsClass_H_
#define GraphicsClass_H_

//=================================
// forward declared dependencies
class WindowClass;
class D3DClass;
class CameraClass;
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

	CameraClass* getCamera();

private:
	CameraClass* m_Camera;
	ShaderControllerClass* m_Shader;
	TwoDGraphicsClass* m_2DGraphics;
	ThreeDGraphicsClass* m_3DGraphics;
};

#endif /* GraphicsClass_H_ */