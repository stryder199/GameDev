#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class TextureClass;

class PixelShaderClass
{
public:
    enum ShaderType
    {
        TWOD = 0,
        THREEDMATERIAL = 1,
        THREEDTEXTURE = 2,
        TEXT = 3
    };

private:
    struct MatrixBufferType
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 projection;
    };

    struct LightBufferType
    {
        DirectX::XMFLOAT4 ambientColor;
        DirectX::XMFLOAT4 diffuseColor;
        DirectX::XMFLOAT3 lightDirection;
        float padding;
    };

    struct ColorBufferType
    {
        DirectX::XMFLOAT4 color;
    };

public: 
    PixelShaderClass();
    ~PixelShaderClass() {};

    void Initialize(WCHAR* psFilename, ShaderType type);
    void Shutdown();
    void Render(TextureClass* tex, DirectX::XMFLOAT3 lightDir, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse);
    void Render(DirectX::XMFLOAT3 lightDir, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 color);
    void Render(TextureClass* tex, DirectX::XMFLOAT4 color);
    void Render(TextureClass* tex);

    ShaderType getShaderType();
private:
    void InitializeShader(WCHAR* psFilename);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

    void SetShaderParameters(TextureClass* tex, DirectX::XMFLOAT3 lightDir, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse);
    void SetShaderParameters(DirectX::XMFLOAT3 lightDir, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 color);
    void SetShaderParameters(TextureClass* tex, DirectX::XMFLOAT4 color);
    void SetShaderParameters(TextureClass* tex);
    void RenderShader();

private:
    ID3D11PixelShader* m_pixelShader;
    ID3D11Buffer* m_lightBuffer;
    ID3D11Buffer* m_colorBuffer;
    ID3D11SamplerState* m_sampleState;
    ShaderType m_type;
};