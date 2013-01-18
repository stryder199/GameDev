//=================================
// include guard
#ifndef PlayerClass_H_
#define PlayerClass_H_

//=================================
// forward declared dependencies
class MeshClass;
class ShaderClass;
class D3DClass;
class CameraClass;
class TextureClass;
class LightClass;

//=================================
// included dependencies
#include <D3D11.h>
#include <D3DX10math.h>

class PlayerClass{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	PlayerClass();
	~PlayerClass();
	
	bool Initialize(MeshClass*, TextureClass*, ID3D11Device*);
	bool Render(D3DClass* D3D, ShaderClass* shader, CameraClass* camera, LightClass* lightSource);
	
	void Shutdown();


private:
	bool InitializeBuffers(ID3D11Device* device);
	bool RenderBuffers(D3DClass* D3D, ShaderClass* shader, CameraClass* camera, LightClass* lightSource);
	bool PreProcessing();
	void ShutdownBuffers();


	MeshClass* m_mesh;
	TextureClass* m_texture;
	D3DXMATRIX* m_worldMatrix;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	float pos_x, pos_y, pos_z;
	float vel_x, vel_y, vel_z;

};

#endif /* PlayerClass_H_ */