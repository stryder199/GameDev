#include "TextClass.h"
#include "FontClass.h"
#include "MeshClass.h"
#include "MeshDataClass.h"
#include "ObjectMeshClass.h"
#include "CameraClass.h"
#include "PlayerClass.h"

TextClass::TextClass()
{
    m_mesh = 0;
    m_lightSource = 0;
    m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_screenPos = XMFLOAT2(0.0f, 0.0f);
    m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


TextClass::~TextClass()
{
}

void TextClass::Initialize(string initText, FontClass* font, XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color)
{
    m_font = font;
    m_text = initText;
    m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_screenPos = pos;
    m_scale = XMFLOAT3(scale.x, scale.y, 1.0f);
    m_color = color;

    BuildTextMesh(initText.c_str());
}

void TextClass::Shutdown()
{
    if (m_mesh)
    {
        m_mesh->Shutdown();
        delete m_mesh;
        m_mesh = 0;
    }
    return;
}

void TextClass::Render(ShaderControllerClass* shader)
{
    ModelClass::RenderBuffers(shader);
}

void TextClass::UpdateText(string newText)
{
    if (m_text.compare(newText) != 0)
    {
        m_text = newText;
        BuildTextMesh(m_text.c_str());
    }
}

void TextClass::PreProcessing()
{
    m_rot = CameraClass::getInstance()->getRotation();
    m_pos.x = PlayerClass::getInstance()->getPosition().x;
    m_pos.y = PlayerClass::getInstance()->getPosition().y;
    m_pos.z = PlayerClass::getInstance()->getPosition().z;

    m_point_pos.x = m_screenPos.x;
    m_point_pos.y = m_screenPos.y;

    ConstrainRotation();

    CalculateWorldMatrix();
}

void TextClass::BuildTextMesh(const char* sentence)
{
    ObjectMeshClass *textObject = new ObjectMeshClass();
    MeshDataClass *meshData = new MeshDataClass();
    int letter;
    float drawX = 0.0f;
    float maxY = 15.0f, minY = 0.0f;

    if (m_mesh)
    {
        m_mesh->Shutdown();
        delete m_mesh;
        m_mesh = 0;
    }

    m_mesh = new MeshClass();

    textObject->Initialize("Text");
    meshData->Initialize(m_font->GetTexture());
    textObject->addMesh(meshData);

    // Draw each letter onto a quad.
    for (int i = 0; i < (int) strlen(sentence); i++)
    {
        letter = ((int) sentence[i]) - 32;

        // If the letter is a space then just move over three pixels.
        if (letter == 0)
        {
            drawX = drawX + 3.0f;
        }
        else
        {
            MeshDataClass::MeshDataType tri_bottomLeft, tri_bottomRight, tri_topLeft, tri_topRight;
            // First triangle in quad.
            // Bottom left
            tri_bottomLeft.x = drawX;
            tri_bottomLeft.y = minY;
            tri_bottomLeft.z = 0.0f;
            tri_bottomLeft.tu = m_font->GetFont()[letter].left;
            tri_bottomLeft.tv = 1.0f;
            tri_bottomLeft.nx = 0.0f;
            tri_bottomLeft.ny = 0.0f;
            tri_bottomLeft.nz = 1.0f;
            
            // Bottom right
            tri_bottomRight.x = drawX + m_font->GetFont()[letter].size;
            tri_bottomRight.y = minY;
            tri_bottomRight.z = 0.0f;
            tri_bottomRight.tu = m_font->GetFont()[letter].right;
            tri_bottomRight.tv = 1.0f;
            tri_bottomRight.nx = 0.0f;
            tri_bottomRight.ny = 0.0f;
            tri_bottomRight.nz = 1.0f;

            // Top Left
            tri_topLeft.x = drawX;
            tri_topLeft.y = maxY;
            tri_topLeft.z = 0.0f;
            tri_topLeft.tu = m_font->GetFont()[letter].left;
            tri_topLeft.tv = 0.0f;
            tri_topLeft.nx = 0.0f;
            tri_topLeft.ny = 0.0f;
            tri_topLeft.nz = 1.0f;

            // Top right
            tri_topRight.x = drawX + m_font->GetFont()[letter].size;
            tri_topRight.y = maxY;
            tri_topRight.z = 0.0f;
            tri_topRight.tu = m_font->GetFont()[letter].right;
            tri_topRight.tv = 0.0f;
            tri_topRight.nx = 0.0f;
            tri_topRight.ny = 0.0f;
            tri_topRight.nz = 1.0f;

            // First triangle in quad.
            // Top left
            meshData->addMeshData(tri_topLeft);

            // Bottom right
            meshData->addMeshData(tri_bottomRight);

            // Bottom left
            meshData->addMeshData(tri_bottomLeft);

            // Second triangle in quad.
            // Top left
            meshData->addMeshData(tri_topLeft);

            // Top right
            meshData->addMeshData(tri_topRight);

            // Bottom right
            meshData->addMeshData(tri_bottomRight);

            // Update the x location for drawing by the size of the letter and one pixel.
            drawX = drawX + m_font->GetFont()[letter].size + 1.0f;
        }
    }

    m_mesh->Initialize(textObject, MeshClass::TEXT);
}
