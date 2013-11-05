////////////////////////////////////////////////////////////////////////////////
// Filename: PixelShaderClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PixelShaderClass.h"
#include "D3DClass.h"
#include "WindowClass.h"
#include "TextureClass.h"

PixelShaderClass::PixelShaderClass()
{
	m_pixelShader = 0;
	m_lightBuffer = 0;
	m_colorBuffer = 0;
	m_sampleState = 0;
}


PixelShaderClass::PixelShaderClass(const PixelShaderClass& other)
{
}

PixelShaderClass::~PixelShaderClass()
{
}

bool PixelShaderClass::Initialize(WCHAR* psFilename, PixelShaderClass::ShaderType type)
{
	bool result;

	m_type = type;

	//Initialize the vertex and pixel shaders
	result = InitializeShader(psFilename);
	if(!result)
		return false;

	return true;
}

void PixelShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	ShutdownShader();

	return;
}

bool PixelShaderClass::Render(TextureClass* texture,  XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(texture, lightDirection, ambientColor, diffuseColor);
	if(!result)
		return false;

	//Now render the prepared buffers with the shader.
	RenderShader();

	return true;
}

bool PixelShaderClass::Render(XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 color)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(lightDirection, ambientColor, diffuseColor, color);
	if (!result)
		return false;

	//Now render the prepared buffers with the shader.
	RenderShader();

	return true;
}

bool PixelShaderClass::Render(TextureClass* texture, XMFLOAT4 color)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(texture, color);
	if (!result)
		return false;

	//Now render the prepared buffers with the shader.
	RenderShader();

	return true;
}

bool PixelShaderClass::Render(TextureClass* texture)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(texture);
	if (!result)
		return false;

	//Now render the prepared buffers with the shader.
	RenderShader();

	return true;
}

bool PixelShaderClass::InitializeShader(WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_BUFFER_DESC lightBufferDesc, colorBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	pixelShaderBuffer = 0;

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "MyPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, psFilename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(WindowClass::getInstance()->gethWnd(), psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//Create the pixel shader from the buffer
	result = D3DClass::getInstance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
		return false;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	if (m_type == THREEDTEXTURE || m_type == TWOD || m_type == TEXT)
	{
		//Create a texture sample state desc
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		//Create the texture sampler state
		result = D3DClass::getInstance()->GetDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);
		if(FAILED(result))
		{
			return false;
		}
	}

	if (m_type == THREEDTEXTURE || m_type == THREEDMATERIAL)
	{
		// Setup the description of the light dynamic constant buffer that is in the pixel shader.
		// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBufferType);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
		if (FAILED(result))
			return false;
	}

	if (m_type == TEXT || m_type == THREEDMATERIAL)
	{
		// Setup the description of the light dynamic constant buffer that is in the pixel shader.
		// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
		colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		colorBufferDesc.ByteWidth = sizeof(ColorBufferType);
		colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		colorBufferDesc.MiscFlags = 0;
		colorBufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&colorBufferDesc, NULL, &m_colorBuffer);
		if (FAILED(result))
			return false;
	}

	return true;
}

void PixelShaderClass::ShutdownShader()
{
	// Release the sampler state.
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the light constant buffer.
	if(m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	return;
}

void PixelShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(WindowClass::getInstance()->gethWnd(), L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool PixelShaderClass::SetShaderParameters(TextureClass* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightBufferType* dataPtr;
	unsigned int bufferNumber;


	ID3D11ShaderResourceView* textureResource = texture->GetTexture();
	// Set shader texture resource in the pixel shader.
	D3DClass::getInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &textureResource);

	//Lock the light constant buffer
	result = D3DClass::getInstance()->GetDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (LightBufferType*) mappedResource.pData;

	//Set the buffer values
	dataPtr->ambientColor = ambientColor;
	dataPtr->diffuseColor = diffuseColor;
	dataPtr->lightDirection = lightDirection;
	dataPtr->padding = 0.0f;

	//Unlock the buffer
	D3DClass::getInstance()->GetDeviceContext()->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	D3DClass::getInstance()->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

bool PixelShaderClass::SetShaderParameters(TextureClass* texture, XMFLOAT4 color)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ColorBufferType* dataPtr;
	unsigned int bufferNumber;

	ID3D11ShaderResourceView* textureResource = texture->GetTexture();
	// Set shader texture resource in the pixel shader.
	D3DClass::getInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &textureResource);

	//Lock the color constant buffer
	result = D3DClass::getInstance()->GetDeviceContext()->Map(m_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (ColorBufferType*) mappedResource.pData;

	//Set the buffer values
	dataPtr->color = color;

	//Unlock the buffer
	D3DClass::getInstance()->GetDeviceContext()->Unmap(m_colorBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	D3DClass::getInstance()->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 1, &m_colorBuffer);

	return true;
}

bool PixelShaderClass::SetShaderParameters(TextureClass* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	ID3D11ShaderResourceView* textureResource = texture->GetTexture();
	// Set shader texture resource in the pixel shader.
	D3DClass::getInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &textureResource);

	return true;
}


bool PixelShaderClass::SetShaderParameters(XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT4 color)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource, mappedResource2;
	LightBufferType* dataPtr;
	ColorBufferType* dataPtr2;
	unsigned int bufferNumber;

	//Lock the light constant buffer
	result = D3DClass::getInstance()->GetDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (LightBufferType*) mappedResource.pData;

	//Set the buffer values
	dataPtr->ambientColor = ambientColor;
	dataPtr->diffuseColor = diffuseColor;
	dataPtr->lightDirection = lightDirection;
	dataPtr->padding = 0.0f;

	//Unlock the buffer
	D3DClass::getInstance()->GetDeviceContext()->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	D3DClass::getInstance()->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 2, &m_lightBuffer);

	bufferNumber++;

	//Lock the light constant buffer
	result = D3DClass::getInstance()->GetDeviceContext()->Map(m_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (ColorBufferType*) mappedResource2.pData;

	//Set the buffer values
	dataPtr2->color = color;

	//Unlock the buffer
	D3DClass::getInstance()->GetDeviceContext()->Unmap(m_colorBuffer, 0);

	// Finally set the light constant buffer in the pixel shader with the updated values.
	D3DClass::getInstance()->GetDeviceContext()->PSSetConstantBuffers(bufferNumber, 2, &m_colorBuffer);

	return true;
}

void PixelShaderClass::RenderShader()
{
	D3DClass::getInstance()->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	if (m_type == THREEDTEXTURE || m_type == TWOD || m_type == TEXT)
	{
		//Set the sampler state in the pixel shader
		D3DClass::getInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);
	}

	return;
}

PixelShaderClass::ShaderType PixelShaderClass::getShaderType()
{
	return m_type;
}