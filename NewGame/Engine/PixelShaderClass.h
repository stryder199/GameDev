////////////////////////////////////////////////////////////////////////////////
// Filename: PixelShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PixelShaderClass_H_
#define _PixelShaderClass_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>
#include <DirectXMath.h>
#include <fstream>

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
		THREEDTEXTURE = 2
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

public: 
	PixelShaderClass();
	PixelShaderClass(const PixelShaderClass&);
	~PixelShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd, WCHAR* psFilename, ShaderType type);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT4);

	ShaderType getShaderType();
private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11PixelShader* m_pixelShader;
	ID3D11Buffer* m_lightBuffer;
	ID3D11SamplerState* m_sampleState;
	ShaderType m_type;
};

#endif