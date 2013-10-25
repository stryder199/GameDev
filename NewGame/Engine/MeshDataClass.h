#pragma once

#include <vector>
#include <D3D11.h>
#include <DirectXMath.h>

class MaterialClass;
class TextureClass;

using namespace std;

class MeshDataClass
{
public:
	struct MeshType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	enum MeshColorType
	{
		MATERIAL = 0,
		TEXTURE = 1,
		NONE = 2
	};

public:
	MeshDataClass();
	~MeshDataClass();

	bool Initialize(MaterialClass* mat);
	bool Initialize(TextureClass* tex);
	void Shutdown();

	int getIndexCount();
	int getVertexCount();
	MeshColorType getMeshColorType();
	MaterialClass* getMaterial();
	TextureClass* getTexture();
	vector<MeshType>* getRawMeshData();
	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getColorBuffer();

	void setVertexBuffer(ID3D11Buffer* buf);
	void setIndexBuffer(ID3D11Buffer* buf);
	void setColorBuffer(ID3D11Buffer* buf);

	void addMeshData(MeshType data);

private:
	int m_vertexCount, m_indexCount;
	vector<MeshType> m_vecMesh;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_colorBuffer;

	MaterialClass* m_material;
	TextureClass* m_texture;
	MeshColorType m_type;
};