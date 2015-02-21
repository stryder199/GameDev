#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
#include "VertexShaderClass.h"
#include "PixelShaderClass.h"

/////////////////
// FORWARD DEC //

class ShaderControllerClass{

public:
    ShaderControllerClass();
    ~ShaderControllerClass();

    void Initialize();
    void Shutdown();

    void Render(int indexCount, const DirectX::XMFLOAT4X4& worldMatrix, TextureClass* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor);

    void Render(int indexCount, const DirectX::XMFLOAT4X4& worldMatrix, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT4 color);

    void Render(int indexCount, const DirectX::XMFLOAT4X4& worldMatrix, TextureClass* texture, DirectX::XMFLOAT4 color);

    void Render(int indexCount, const DirectX::XMFLOAT4X4& worldMatrix, TextureClass* texture);

    void Set3DMaterialShaders();
    void Set3DTextureShaders();
    void Set2DShaders();
    void SetTextShaders();

    VertexShaderClass* GetFocusVertex();
    PixelShaderClass* GetFocusPixel();

private:
    VertexShaderClass* m_vertexFocus;
    PixelShaderClass* m_pixelFocus;

    VertexShaderClass m_twoDVS;
    PixelShaderClass m_twoDPS;

    VertexShaderClass m_textVS;
    PixelShaderClass m_textPS;

    VertexShaderClass m_threeDMaterialVS;
    PixelShaderClass m_threeDMaterialPS;

    VertexShaderClass m_threeDTextureVS;
    PixelShaderClass m_threeDTexturePS;
};