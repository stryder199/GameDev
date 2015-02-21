#pragma once

//////////////
// INCLUDES //
//////////////
#include <vector>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ObjectMeshClass;

class MeshClass
{
public:
    enum MeshType
    {
        THREED = 0,
        TWOD = 1,
        TEXT = 2,
        NONE = 3
    };

    struct VertexTextureType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texture;
        DirectX::XMFLOAT3 normals;
    };

    struct VertexMaterialType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normals;
    };

    struct VertexTextType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texture;
    };

public:
    MeshClass() {};
    ~MeshClass() {};
    void Initialize(std::string filename, MeshType type);
    void Initialize(ObjectMeshClass *object, MeshType type);
    void Shutdown();


    MeshType getMeshType();
    std::vector<ObjectMeshClass*>* getAllObjects();
    std::vector<DirectX::XMFLOAT3>* getGuns();

private:
    void LoadModel(std::string filename);
    void InitializeBuffers();

    std::vector<ObjectMeshClass*> m_allObjects;
    std::vector<DirectX::XMFLOAT3> m_guns;
    MeshType m_type;
};