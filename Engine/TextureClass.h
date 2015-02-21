#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

class TextureClass
{
public:
    TextureClass();
    ~TextureClass();

    void Initialize(LPCWSTR);
    void Shutdown();

    ID3D11ShaderResourceView* GetTexture();

private:
    ID3D11ShaderResourceView* m_texture;
};
