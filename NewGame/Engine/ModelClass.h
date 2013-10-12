//=================================
// include guard
#ifndef ModelClass_H_
#define ModelClass_H_

//=================================
// forward declared dependencies
class MeshClass;
class TextureClass;
class D3DClass;
class ShaderControllerClass;
class CameraClass;
class LightClass;

//=================================
// included dependencies
#include <D3D11.h>
#include <DirectXMath.h>

using namespace DirectX;

class ModelClass{
protected:
	struct VertexTextureType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normals;
	};

	struct VertexMaterialType
	{
		XMFLOAT3 position;
		XMFLOAT3 normals;
		XMFLOAT4 color;
	};

public:
	virtual bool Initialize(MeshClass* mesh, TextureClass* texture, ID3D11Device* device) { return false; }
	virtual bool Render(D3DClass* D3D, ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource) { return false; }
	virtual void Shutdown() {}

protected:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();

	MeshClass* m_mesh;
	TextureClass* m_texture;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_colorBuffer;
	float pos_x, pos_y, pos_z;

};

#endif /* ModelClass_H_ */