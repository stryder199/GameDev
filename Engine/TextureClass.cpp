#include "TextureClass.h"
#include "D3DClass.h"
#include "GenericException.h"
#include "D3DX11.h"

using namespace std;
using namespace DirectX;

TextureClass::TextureClass()
{
    m_texture = nullptr;
}

TextureClass::~TextureClass()
{
}

void TextureClass::Initialize(LPCWSTR filename)
{
    HRESULT result = S_OK;

    //Load the texture
    result = D3DX11CreateShaderResourceViewFromFile(D3DClass::getInstance()->GetDevice(), filename, nullptr, nullptr, &m_texture, nullptr);
    if(FAILED(result))
    {
        throw GenericException("Failed to create texture.");
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