#include "PlanetClass.h"
#include "ModelClass.h"
#include "CameraClass.h"
#include "LightClass.h"

using namespace std;
using namespace DirectX;

PlanetClass::PlanetClass()
    : ModelClass()
{
}

PlanetClass::~PlanetClass()
{
}

void PlanetClass::Initialize(MeshClass* objMesh, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel)
{
    m_mesh = objMesh;
    m_pos = pos;
    m_scale = scale;
    m_rotVel = rotVel;

    // Create the light object.
    m_lightSource = new LightClass();
    // Initialize the light object.
    m_lightSource->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    m_lightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightSource->SetDirection(0.0f, 0.0f, 1.0f);
}

void PlanetClass::Shutdown()
{
    if (m_lightSource)
    {
        delete m_lightSource;
        m_lightSource = 0;
    }

    return;
}

void PlanetClass::Render(ShaderControllerClass* shader)
{
    RenderBuffers(shader);
}

void PlanetClass::PreProcessing()
{
}
