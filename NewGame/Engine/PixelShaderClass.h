////////////////////////////////////////////////////////////////////////////////
// Filename: PixelShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>
#include <fstream>

class TextureClass;

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: PixelShaderClass
////////////////////////////////////////////////////////////////////////////////
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
	PixelShaderClass(const PixelShaderClass&);
	~PixelShaderClass();

	bool Initialize(WCHAR* psFilename, ShaderType type);
	void Shutdown();
	bool Render(TextureClass* tex, XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse);
	bool Render(XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 color);
	bool Render(TextureClass* tex, XMFLOAT4 color);
	bool Render(TextureClass* tex);

	ShaderType getShaderType();
private:
	bool InitializeShader(WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool SetShaderParameters(TextureClass* tex, XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse);
	bool SetShaderParameters(XMFLOAT3 lightDir, XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 color);
	bool SetShaderParameters(TextureClass* tex, XMFLOAT4 color);
	bool SetShaderParameters(TextureClass* tex);
	void RenderShader();

private:
	ID3D11PixelShader* m_pixelShader;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_colorBuffer;
	ID3D11SamplerState* m_sampleState;
	ShaderType m_type;
};