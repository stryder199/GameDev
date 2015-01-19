#include "ThreeDGraphicsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderControllerClass.h"
#include "LightClass.h"
#include "ModelClass.h"
#include "StarClass.h"
#include "PlanetClass.h"
#include "PlayerClass.h"
#include "MeshControllerClass.h"
#include "WindowClass.h"

ThreeDGraphicsClass::ThreeDGraphicsClass()
{
}

ThreeDGraphicsClass::~ThreeDGraphicsClass()
{
}

void ThreeDGraphicsClass::Initialize()
{
    m_allModels = vector<ModelClass*>();
}

void ThreeDGraphicsClass::Shutdown()
{
    modelMutex.lock();
    vector<ModelClass*>::iterator model;
    for (model = m_allModels.begin(); model != m_allModels.end(); ++model)
    {
        (*model)->Shutdown();
    }
    modelMutex.unlock();
}

void ThreeDGraphicsClass::RenderAll(ShaderControllerClass* shader)
{
    ConstructFrustum();

    modelMutex.lock();
    vector<ModelClass*>::iterator model;
    for (model = m_allModels.begin(); model != m_allModels.end(); ++model)
    {
        (*model)->PreProcessing();
    }
    modelMutex.unlock();

    CameraClass::getInstance()->Render();

    modelMutex.lock();
    for (model = m_allModels.begin(); model != m_allModels.end(); ++model)
    {
        if (CheckSphereAgainstFrustum((*model)->getPosition(), (*model)->getBasicCollisionCircleRadius()))
        {
            (*model)->Render(shader);
        }
    }
    modelMutex.unlock();
}

void ThreeDGraphicsClass::AddPlayer(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos)
{
    PlayerClass *player = PlayerClass::getInstance();
    MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);

    player->Initialize(mesh, pos, scale, totalHealth, totalShields, totalEnergy, energyCost, torpedos);

    modelMutex.lock();
    m_allModels.push_back(player);
    modelMutex.unlock();
}

void ThreeDGraphicsClass::AddStar(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
    StarClass *star = new StarClass();
    MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);

    star->Initialize(mesh, pos, scale, rotVel);

    modelMutex.lock();
    m_allModels.push_back(star);
    modelMutex.unlock();
}

void ThreeDGraphicsClass::AddPlanet(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
    PlanetClass *planet = new PlanetClass();
    MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);

    planet->Initialize(mesh, pos, scale, rotVel);

    modelMutex.lock();
    m_allModels.push_back(planet);
    modelMutex.unlock();
}

void ThreeDGraphicsClass::ConstructFrustum()
{
    float zMinimum, r;
    XMMATRIX matrix;
    XMFLOAT4X4 matrixFloat;
    XMVECTOR normalized;

    XMFLOAT4X4 projectionFloat4x4 = D3DClass::getInstance()->GetProjectionMatrix();
    XMFLOAT4X4 viewFloat4x4 = CameraClass::getInstance()->GetViewMatrix();
    float screenDepth = WindowClass::getInstance()->SCREEN_DEPTH;

    // Calculate the minimum Z distance in the frustum.
    zMinimum = -projectionFloat4x4._43 / projectionFloat4x4._33;
    r = screenDepth / (screenDepth - zMinimum);
    projectionFloat4x4._33 = r;
    projectionFloat4x4._43 = -r * zMinimum;
    XMMATRIX projMat = XMLoadFloat4x4(&projectionFloat4x4);
    XMMATRIX viewMat = XMLoadFloat4x4(&viewFloat4x4);

    // Create the frustum matrix from the view matrix and updated projection matrix.
    matrix = XMMatrixMultiply(viewMat, projMat);

    XMStoreFloat4x4(&matrixFloat, matrix);

    // Calculate near plane of frustum.
    m_fulstrumPlanes[0].x = matrixFloat._14 + matrixFloat._13;
    m_fulstrumPlanes[0].y = matrixFloat._24 + matrixFloat._23;
    m_fulstrumPlanes[0].z = matrixFloat._34 + matrixFloat._33;
    m_fulstrumPlanes[0].w = matrixFloat._44 + matrixFloat._43;
    XMVECTOR plane0 = XMLoadFloat4(&m_fulstrumPlanes[0]);
    normalized = XMPlaneNormalize(plane0);
    XMStoreFloat4(&m_fulstrumPlanes[0], normalized);

    // Calculate far plane of frustum.
    m_fulstrumPlanes[1].x = matrixFloat._14 - matrixFloat._13;
    m_fulstrumPlanes[1].y = matrixFloat._24 - matrixFloat._23;
    m_fulstrumPlanes[1].z = matrixFloat._34 - matrixFloat._33;
    m_fulstrumPlanes[1].w = matrixFloat._44 - matrixFloat._43;
    XMVECTOR plane1 = XMLoadFloat4(&m_fulstrumPlanes[1]);
    normalized = XMPlaneNormalize(plane1);
    XMStoreFloat4(&m_fulstrumPlanes[1], normalized);

    // Calculate left plane of frustum.
    m_fulstrumPlanes[2].x = matrixFloat._14 + matrixFloat._11;
    m_fulstrumPlanes[2].y = matrixFloat._24 + matrixFloat._21;
    m_fulstrumPlanes[2].z = matrixFloat._34 + matrixFloat._31;
    m_fulstrumPlanes[2].w = matrixFloat._44 + matrixFloat._41;
    XMVECTOR plane2 = XMLoadFloat4(&m_fulstrumPlanes[2]);
    normalized = XMPlaneNormalize(plane2);
    XMStoreFloat4(&m_fulstrumPlanes[2], normalized);

    // Calculate right plane of frustum.
    m_fulstrumPlanes[3].x = matrixFloat._14 - matrixFloat._11;
    m_fulstrumPlanes[3].y = matrixFloat._24 - matrixFloat._21;
    m_fulstrumPlanes[3].z = matrixFloat._34 - matrixFloat._31;
    m_fulstrumPlanes[3].w = matrixFloat._44 - matrixFloat._41;
    XMVECTOR plane3 = XMLoadFloat4(&m_fulstrumPlanes[3]);
    normalized = XMPlaneNormalize(plane3);
    XMStoreFloat4(&m_fulstrumPlanes[3], normalized);

    // Calculate top plane of frustum.
    m_fulstrumPlanes[4].x = matrixFloat._14 - matrixFloat._12;
    m_fulstrumPlanes[4].y = matrixFloat._24 - matrixFloat._22;
    m_fulstrumPlanes[4].z = matrixFloat._34 - matrixFloat._32;
    m_fulstrumPlanes[4].w = matrixFloat._44 - matrixFloat._42;
    XMVECTOR plane4 = XMLoadFloat4(&m_fulstrumPlanes[4]);
    normalized = XMPlaneNormalize(plane4);
    XMStoreFloat4(&m_fulstrumPlanes[4], normalized);

    // Calculate bottom plane of frustum.
    m_fulstrumPlanes[5].x = matrixFloat._14 + matrixFloat._12;
    m_fulstrumPlanes[5].y = matrixFloat._24 + matrixFloat._22;
    m_fulstrumPlanes[5].z = matrixFloat._34 + matrixFloat._32;
    m_fulstrumPlanes[5].w = matrixFloat._44 + matrixFloat._42;
    XMVECTOR plane5 = XMLoadFloat4(&m_fulstrumPlanes[5]);
    normalized = XMPlaneNormalize(plane5);
    XMStoreFloat4(&m_fulstrumPlanes[5], normalized);
}

bool ThreeDGraphicsClass::CheckSphereAgainstFrustum(XMFLOAT3 pos, float radius)
{
    int i;
    XMVECTOR posVec = XMLoadFloat3(&pos);

    // Check if the radius of the sphere is inside the view frustum.
    for (i = 0; i<6; i++)
    {
        XMVECTOR planeVec = XMLoadFloat4(&m_fulstrumPlanes[i]);
        float result = 0.0f;
        XMStoreFloat(&result, XMPlaneDotCoord(planeVec, posVec));
        if (result < -radius)
        {
            return false;
        }
    }

    return true;
}