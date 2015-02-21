#include "StarClass.h"
#include "MeshClass.h"
#include "LightClass.h"

using namespace std;
using namespace DirectX;

StarClass::StarClass()
    : ModelClass()
{
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
    RenderBuffers(shader);
}

void StarClass::PreProcessing()
{
}