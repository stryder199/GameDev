//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class BitmapClass;
class D3DClass;
class CameraClass;
class ShaderControllerClass;
class LightClass;

//=================================
// included dependencies
#include <d3d11.h>

class TwoDGraphicsClass{

public:
	TwoDGraphicsClass();
	TwoDGraphicsClass(const TwoDGraphicsClass&);
	~TwoDGraphicsClass();

	bool Initialize(int screenWidth, int screenHeight);

	bool RenderAll(ShaderControllerClass* shader);

private:
	BitmapClass* m_Bitmap;
	LightClass* m_DefaultLightSource;
};