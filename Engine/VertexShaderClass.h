#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

class VertexShaderClass
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

public: 
    VertexShaderClass();
    ~VertexShaderClass();

    void Initialize(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount, ShaderType type);
    void Shutdown();
    void Render(int, const DirectX::XMFLOAT4X4 &worldMatrix);

    ShaderType getShaderType();
private:
    void InitializeShader(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

    void SetShaderParameters(const DirectX::XMFLOAT4X4 &worldMatrix);
    void RenderShader(int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;
    ShaderType m_type;
};