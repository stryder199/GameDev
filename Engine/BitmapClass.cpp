////////////////////////////////////////////////////////////////////////////////
// Filename: bitmapclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "BitmapClass.h"
#include "CameraClass.h"

BitmapClass::BitmapClass()
{
    m_mesh = 0;
    m_lightSource = 0;
    m_screenPos = XMFLOAT2(0.0f, 0.0f);
    m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

BitmapClass::~BitmapClass()
{
}

void BitmapClass::Initialize(MeshClass* mesh, LightClass *lightSource, XMFLOAT2 pos, XMFLOAT2 scale)
{
    m_mesh = mesh;
    m_lightSource = lightSource;
    m_screenPos = pos;
    m_scale = XMFLOAT3(scale.x, scale.y, 1.0f);
}

void BitmapClass::Shutdown()
{
    return;
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