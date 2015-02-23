////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "BitmapClass.h"
#include "CameraClass.h"

using namespace std;
using namespace DirectX;

BitmapClass::BitmapClass()
    : ModelClass()
{
}

BitmapClass::~BitmapClass()
{
}

void BitmapClass::Initialize(shared_ptr<MeshClass> mesh, LightClass *lightSource, XMFLOAT2 pos, XMFLOAT2 scale)
{
    m_mesh = mesh;
    m_lightSource = lightSource;
    m_screenPos = pos;
    m_scale = XMFLOAT3(scale.x, scale.y, 1.0f);
}

void BitmapClass::Render(ShaderControllerClass *shader)
{
    ModelClass::RenderBuffers(shader);
}

void BitmapClass::PreProcessing()
{
    m_rot = CameraClass::getInstance()->getRotation();

    m_pos.x = CameraClass::getInstance()->getPosition().x;
    m_pos.y = CameraClass::getInstance()->getPosition().y;
    m_pos.z = CameraClass::getInstance()->getPosition().z;

    m_point_pos.x = m_screenPos.x;
    m_point_pos.y = m_screenPos.y;
}