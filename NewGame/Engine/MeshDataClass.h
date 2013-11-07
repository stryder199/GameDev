#pragma once

//////////////
// INCLUDES //
#include <vector>
#include <D3D11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class MaterialClass;
class TextureClass;

using namespace std;

class MeshDataClass
{
public:
	struct MeshDataType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	enum MeshColorType
	{
		TEXTURE = 0,
		MATERIAL = 1
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
	vector<MeshDataType>* getRawMeshData();
	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getColorBuffer();

	void setVertexBuffer(ID3D11Buffer* buf);
	void setIndexBuffer(ID3D11Buffer* buf);
	void setColorBuffer(ID3D11Buffer* buf);

	void addMeshData(MeshDataType data);

private:
	int m_vertexCount, m_indexCount;
	vector<MeshDataType> m_vecMesh;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_colorBuffer;

	MaterialClass* m_material;
	TextureClass* m_texture;
	MeshColorType m_colorType;
};