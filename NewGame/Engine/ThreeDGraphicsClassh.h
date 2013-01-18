//=================================
// include guard
#ifndef ThreeDGraphicsClass_H_
#define ThreeDGraphicsClass_H_

//=================================
// forward declared dependencies
class ActorsClass;
class EnvironmentArtClass;
class ShaderClass;
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

	bool Initialize(ID3D11Device* device);

	bool RenderAll(D3DClass* D3D, ShaderClass* shader, CameraClass* camera);

private:
	EnvironmentArtClass* envArt;
	ActorsClass* actors;
	LightClass* m_LightSource;
};

#endif /* ThreeDGraphicsClass_H_ */