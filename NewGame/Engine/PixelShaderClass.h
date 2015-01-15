#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>
#include <fstream>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class TextureClass;

using namespace DirectX;

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
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

	struct ColorBufferType
	{
		XMFLOAT4 color;
	};

public: 
	PixelShaderClass();
	~PixelShaderClass() {};

    void Initialize(WCHAR* psFilename, ShaderType type);
	void Shutdown();
    void Render(TextureClass* tex, XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse);
    void Render(XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 color);
    void Render(TextureClass* tex, XMFLOAT4 color);
    void Render(TextureClass* tex);

	ShaderType getShaderType();
private:
    void InitializeShader(WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

    void SetShaderParameters(TextureClass* tex, XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse);
    void SetShaderParameters(XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 color);
    void SetShaderParameters(TextureClass* tex, XMFLOAT4 color);
    void SetShaderParameters(TextureClass* tex);
	void RenderShader();

private:
	ID3D11PixelShader* m_pixelShader;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_colorBuffer;
	ID3D11SamplerState* m_sampleState;
	ShaderType m_type;
};