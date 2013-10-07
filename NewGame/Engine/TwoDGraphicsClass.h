//=================================
// include guard
#ifndef TwoDGraphicsClass_H_
#define TwoDGraphicsClass_H_

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

	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight);

	bool RenderAll(D3DClass* D3D, CameraClass* camera, ShaderControllerClass* shader);

private:
	BitmapClass* m_Bitmap;
	LightClass* m_DefaultLightSource;
};

#endif /* TwoDGraphicsClass_H_ */