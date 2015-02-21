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

    void Initialize(MaterialClass* mat);
    void Initialize(TextureClass* tex);
    void Shutdown();

    int getIndexCount();
    int getVertexCount();
    MeshColorType getMeshColorType();
    MaterialClass* getMaterial();
    TextureClass* getTexture();
    std::vector<MeshDataType>* getRawMeshData();
    ID3D11Buffer* getVertexBuffer();
    ID3D11Buffer* getIndexBuffer();
    ID3D11Buffer* getColorBuffer();

    void setVertexBuffer(ID3D11Buffer* buf);
    void setIndexBuffer(ID3D11Buffer* buf);
    void setColorBuffer(ID3D11Buffer* buf);

    void addMeshData(MeshDataType data);

private:
    int m_vertexCount, m_indexCount;
    std::vector<MeshDataType> m_vecMesh;
    ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_colorBuffer;

    MaterialClass* m_material;
    TextureClass* m_texture;
    MeshColorType m_colorType;
};