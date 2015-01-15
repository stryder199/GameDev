#include "TextureClass.h"
#include "D3DClass.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::~TextureClass()
{
}

void TextureClass::Initialize(WCHAR* filename)
{
	HRESULT result;

	//Load the texture
	result = D3DX11CreateShaderResourceViewFromFile(D3DClass::getInstance()->GetDevice(), filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		throw new exception
	}
}

void TextureClass::Shutdown()
{
	// FIX ME WHEN TEXTURE CONTROLLER IS IMP
	if(m_texture)
	{
		//m_texture->Release();
		//m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}