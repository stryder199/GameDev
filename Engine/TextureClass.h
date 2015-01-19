#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <d3dx11tex.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

using namespace std;

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
