#include "ModelClass.h"
#include "MeshClass.h"
#include "MeshDataClass.h"
#include "MaterialClass.h"
#include "ObjectMeshClass.h"
#include "D3DClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "GenericException.h"
#include "VectorHelpers.h"

using namespace std;
using namespace DirectX;

ModelClass::ModelClass()
{
    m_mesh = nullptr;
    m_lightSource = nullptr;
    m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
    m_dirVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_vel = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rotVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void ModelClass::RenderBuffers(ShaderControllerClass* shader)
{
    // For each object in the mesh
    vector<ObjectMeshClass*>::iterator object;
    vector<ObjectMeshClass*>* allObject = m_mesh->getAllObjects();
    for (object = allObject->begin(); object != allObject->end(); ++object)
    {
        // For each submesh
        vector<MeshDataClass*>::iterator subMesh;
        vector<MeshDataClass*>* allMeshData = (*object)->getAllMeshData();
        for (subMesh = allMeshData->begin(); subMesh != allMeshData->end(); ++subMesh)
        {
            unsigned int vertexstride;
            unsigned int offset;

            if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
            {
                vertexstride = sizeof(MeshClass::VertexMaterialType);
                shader->Set3DMaterialShaders();
            }
            else if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
            {
                vertexstride = sizeof(MeshClass::VertexTextureType);
                shader->Set3DTextureShaders();
            }
            else if (m_mesh->getMeshType() == MeshClass::TEXT || m_mesh->getMeshType() == MeshClass::TWOD)
            {
                vertexstride = sizeof(MeshClass::VertexTextType);
            }
            else
            {
                throw GenericException("Failed to acquire the mouse.");
            }

            offset = 0;

            ID3D11Buffer* tmp = (*subMesh)->getVertexBuffer();

            //Set the vertex buffer to active in the input assembler so it can be rendered.
            D3DClass::getInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &tmp, &vertexstride, &offset);

            //Set the index buffer to active in the input assembler so it can be rendered.
            D3DClass::getInstance()->GetDeviceContext()->IASetIndexBuffer((*subMesh)->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

            // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
            D3DClass::getInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
            {
                XMFLOAT4 color = XMFLOAT4((*subMesh)->getMaterial()->getMaterialInfo().Kd_r,
                    (*subMesh)->getMaterial()->getMaterialInfo().Kd_g,
                    (*subMesh)->getMaterial()->getMaterialInfo().Kd_b,
                    1.0f);
                //Render the model using the shader
                shader->Render(
                    (*subMesh)->getIndexCount(), 
                    m_worldMatrix, 
                    m_lightSource->GetDirection(), 
                    m_lightSource->GetAmbientColor(),
                    m_lightSource->GetDiffuseColor(), 
                    color);
            }
            else if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
            {
                //Render the model using the shader
                shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, (*subMesh)->getTexture(), m_lightSource->GetDirection(),
                    m_lightSource->GetAmbientColor(), m_lightSource->GetDiffuseColor());
            }
            else if (m_mesh->getMeshType() == MeshClass::TEXT)
            {
                //Render the model using the shader
                shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, (*subMesh)->getTexture(), m_color);
            }
            else if (m_mesh->getMeshType() == MeshClass::TWOD)
            {
                //Render the model using the shader
                shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, (*subMesh)->getTexture());
            }
            else
            {
                throw GenericException("Unknown Mesh Type");
            }
        }
    }
}

void ModelClass::CalculateWorldMatrix()
{
    // Add velocity and rot velocity
    XMStoreFloat3(&m_pos, XMVectorAdd(XMLoadFloat3(&m_pos), XMLoadFloat3(&m_vel)));
    XMStoreFloat3(&m_rot, XMVectorAdd(XMLoadFloat3(&m_rot), XMLoadFloat3(&m_rotVel)));
    
    XMMATRIX worldMatrix = XMMatrixIdentity();

    // Move the model to the location it should be rendered at.
    XMMATRIX translationMatrix = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
    XMMATRIX pointTranslationMatrix = XMMatrixTranslation(m_point_pos.x, m_point_pos.y, m_point_pos.z);
    XMMATRIX scalingMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    XMMATRIX rotXMatrix = XMMatrixRotationX(m_rot.x);
    XMMATRIX rotYMatrix = XMMatrixRotationY(m_rot.y);
    XMMATRIX rotZMatrix = XMMatrixRotationZ(m_rot.z);

    worldMatrix *= scalingMatrix;
    worldMatrix *= pointTranslationMatrix;
    worldMatrix *= rotXMatrix;
    worldMatrix *= rotYMatrix;
    worldMatrix *= rotZMatrix;
    worldMatrix *= translationMatrix;

    XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
}

void ModelClass::ModelPreProcessing()
{
    VectorHelpers::CalculateDirection(&m_rot, &m_dir);
    XMFLOAT3 prevPosition = m_pos;
    PreProcessing();
    VectorHelpers::ConstrainRotation(&m_rot);
    CalculateWorldMatrix();
    if (!VectorHelpers::Equal(prevPosition, m_pos))
    {
        VectorHelpers::CalculateVelocityDirection(&prevPosition, &m_pos, &m_dirVel);
    }
    else
    {
        VectorHelpers::ZeroVector(&m_dirVel);
    }
}

XMFLOAT3 ModelClass::getPosition()
{
    return m_pos;
}

XMFLOAT3 ModelClass::getRotation()
{
    return m_rot;
}

XMFLOAT3 ModelClass::getScale()
{
    return m_scale;
}

XMFLOAT3 ModelClass::getPointPosition()
{
    return m_point_pos;
}

XMFLOAT3 ModelClass::getDirection()
{
    return m_dir;
}

float ModelClass::getBasicCollisionCircleRadius()
{
    // All models are normalized to be in a 1 by 1 box, thus the scale is the radius of the basic circle
    return m_scale.x;
}