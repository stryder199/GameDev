//=================================
// include guard
#ifndef GraphicsClass_H_
#define GraphicsClass_H_

//=================================
// forward declared dependencies
class WindowClass;
class D3DClass;
class CameraClass;
class ShaderClass;
class ModelClass;
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

	WindowClass* getWindowObj();
	CameraClass* getCamera();

private:
	WindowClass* m_Window;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ShaderClass* m_Shader;
	ModelClass* m_Model;
	TwoDGraphicsClass* m_2DGraphics;
	ThreeDGraphicsClass* m_3DGraphics;


};

#endif /* GraphicsClass_H_ */