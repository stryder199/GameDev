#include "TextClass.h"
#include "FontClass.h"
#include "MeshClass.h"
#include "MeshDataClass.h"
#include "ObjectMeshClass.h"

TextClass::TextClass()
{
	m_mesh = 0;
	m_lightSource = 0;
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_point_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


TextClass::~TextClass()
{
}

bool TextClass::Initialize(std::string initText, FontClass* font, XMFLOAT2 pos, XMFLOAT4 color)
{
	bool result;

	m_font = font;
	m_text = initText;
	m_pos = XMFLOAT3(pos.x, pos.y, 0.0f);
	m_color = color;

	result = BuildTextMesh(initText.c_str());
	if (!result)
		return false;

	return true;
}

void TextClass::Shutdown()
{

}

bool TextClass::Render(ShaderControllerClass* shader)
{
	bool result;

	result = PreProcessing();
	if (!result)
		return false;

	result = ModelClass::RenderBuffers(shader);
	if (!result)
		return false;

	return true;
}

bool TextClass::PreProcessing()
{
	//bool result;

	CalculateWorldMatrix();

	return true;
}

bool TextClass::BuildTextMesh(const char* sentence)
{
	ObjectMeshClass *textObject = new ObjectMeshClass();
	MeshDataClass *meshData = new MeshDataClass();
	int letter;
	int drawX = 0, drawY = 0;
	bool result;

	if (m_mesh)
	{
		m_mesh->Shutdown();
		delete m_mesh;
		m_mesh = 0;
	}

	m_mesh = new MeshClass();

	result = textObject->Initialize("Text");
	if (!result)
		return false;

	result = meshData->Initialize(m_font->GetTexture());
	if (!result)
		return false;

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
			MeshDataClass::MeshDataType tri1_v1, tri1_v2, tri1_v3;
			// First triangle in quad.
			tri1_v1.x = drawX;
			tri1_v1.y = drawY;
			tri1_v1.z = 0.0f;
			tri1_v1.tu = m_font->GetFont()[letter].left;
			tri1_v1.tv = 0.0f;
			tri1_v1.nx = 0.0f;
			tri1_v1.ny = 0.0f;
			tri1_v1.nz = 1.0f;
			meshData->addMeshData(tri1_v1);

			tri1_v2.x = drawX + m_font->GetFont()[letter].size;
			tri1_v2.y = drawY - 16;
			tri1_v2.z = 0.0f;
			tri1_v2.tu = m_font->GetFont()[letter].right;
			tri1_v2.tv = 0.0f;
			tri1_v2.nx = 0.0f;
			tri1_v2.ny = 0.0f;
			tri1_v2.nz = 1.0f;
			meshData->addMeshData(tri1_v2);

			tri1_v3.x = drawX;
			tri1_v3.y = drawY - 16;
			tri1_v3.z = 0.0f;
			tri1_v3.tu = m_font->GetFont()[letter].left;
			tri1_v3.tv = 1.0f;
			tri1_v3.nx = 0.0f;
			tri1_v3.ny = 0.0f;
			tri1_v3.nz = 1.0f;
			meshData->addMeshData(tri1_v3);

			MeshDataClass::MeshDataType tri2_v1, tri2_v2, tri2_v3;

			// Second triangle in quad.
			// Top left
			tri2_v1.x = drawX;
			tri2_v1.y = drawY;
			tri2_v1.z = 0.0f;
			tri2_v1.tu = m_font->GetFont()[letter].left;
			tri2_v1.tv = 0.0f;
			tri2_v1.nx = 0.0f;
			tri2_v1.ny = 0.0f;
			tri2_v1.nz = 1.0f;
			meshData->addMeshData(tri2_v1);

			// Top right
			tri2_v2.x = drawX + m_font->GetFont()[letter].size;
			tri2_v2.y = drawY;
			tri2_v2.z = 0.0f;
			tri2_v2.tu = m_font->GetFont()[letter].right;
			tri2_v2.tv = 0.0f;
			tri2_v2.nx = 0.0f;
			tri2_v2.ny = 0.0f;
			tri2_v2.nz = 1.0f;
			meshData->addMeshData(tri2_v2);

			// Bottom right
			tri2_v3.x = drawX + m_font->GetFont()[letter].size;
			tri2_v3.y = drawY - 16;
			tri2_v3.z = 0.0f;
			tri2_v3.tu = m_font->GetFont()[letter].right;
			tri2_v3.tv = 1.0f;
			tri2_v3.nx = 0.0f;
			tri2_v3.ny = 0.0f;
			tri2_v3.nz = 1.0f;
			meshData->addMeshData(tri2_v3);

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_font->GetFont()[letter].size + 1.0f;
		}
	}

	m_mesh->Initialize(textObject, MeshClass::TEXT);

	result = ModelClass::InitializeBuffers();
	if (!result)
		return false;

	return true;
}
