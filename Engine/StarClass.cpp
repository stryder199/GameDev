#include "StarClass.h"
#include "MeshClass.h"
#include "LightClass.h"

StarClass::StarClass(){
    m_mesh = 0;
    m_lightSource = 0;
    m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rotVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

StarClass::~StarClass(){
}

void StarClass::Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
    m_mesh = objMesh;
    m_pos = pos;
    m_scale = scale;
    m_rotVel = rotVel;

    // Create the light object.
    m_lightSource = new LightClass();
    // Initialize the light object.
    m_lightSource->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_lightSource->SetDirection(0.0f, 0.0f, -1.0f);
}

void StarClass::Shutdown()
{
    if (m_lightSource)
    {
        delete m_lightSource;
        m_lightSource = 0;
    }

    return;
}

void StarClass::Render(ShaderControllerClass* shader)
{
    ModelClass::RenderBuffers(shader);
}

void StarClass::PreProcessing()
{
    m_rot.x += m_rotVel.x;
    m_rot.y += m_rotVel.y;
    m_rot.z += m_rotVel.z;
    ConstrainRotation();

    CalculateWorldMatrix();
}