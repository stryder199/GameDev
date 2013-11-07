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

using namespace DirectX;

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
	VertexShaderClass();
	VertexShaderClass(const VertexShaderClass&);
	~VertexShaderClass();

	bool Initialize(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount, ShaderType type);
	void Shutdown();
	bool Render(int, const XMFLOAT4X4 &worldMatrix);

	ShaderType getShaderType();
private:
	bool InitializeShader(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool SetShaderParameters(const XMFLOAT4X4 &worldMatrix);
	void RenderShader(int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ShaderType m_type;
};