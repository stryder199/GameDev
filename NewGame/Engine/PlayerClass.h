//=================================
// include guard
#ifndef PlayerClass_H_
#define PlayerClass_H_

//=================================
// forward declared dependencies
class MeshClass;
class ShaderControllerClass;
class D3DClass;
class CameraClass;
class TextureClass;
class LightClass;

//=================================
// included dependencies
#include <D3D11.h>
#include <DirectXMath.h>

using namespace DirectX;

class PlayerClass{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normals;
	};

public:
	PlayerClass();
	~PlayerClass();
	
	bool Initialize(MeshClass*, TextureClass*, ID3D11Device*);
	bool Render(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	
	void Shutdown();


private:
	bool InitializeBuffers(ID3D11Device* device);
	bool RenderBuffers(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource);
	bool PreProcessing();
	void ShutdownBuffers();


	MeshClass* m_mesh;
	TextureClass* m_texture;
	XMFLOAT4X4 m_worldMatrix;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	float pos_x, pos_y, pos_z;
	float vel_x, vel_y, vel_z;

};

#endif /* PlayerClass_H_ */