//=================================
// include guard
#ifndef ThreeDGraphicsClass_H_
#define ThreeDGraphicsClass_H_

//=================================
// forward declared dependencies
class ActorsClass;
class EnvironmentArtClass;
class ShaderControllerClass;
class D3DClass;
class LightClass;
class CameraClass;

//=================================
// included dependencies
#include <vector>
#include <D3D11.h>

class ThreeDGraphicsClass{

public:
	ThreeDGraphicsClass();
	ThreeDGraphicsClass(const ThreeDGraphicsClass&);
	~ThreeDGraphicsClass();

	bool Initialize();

	bool RenderAll(ShaderControllerClass* shader, CameraClass* camera);

private:
	EnvironmentArtClass* envArt;
	ActorsClass* actors;
	LightClass* m_LightSource;
};

#endif /* ThreeDGraphicsClass_H_ */