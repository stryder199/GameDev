////////////////////////////////////////////////////////////////////////////////
// Filename: VertexShaderClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "VertexShaderClass.h"

VertexShaderClass::VertexShaderClass()
{
	m_vertexShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}


VertexShaderClass::VertexShaderClass(const VertexShaderClass& other)
{
}

VertexShaderClass::~VertexShaderClass()
{
}

bool VertexShaderClass::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, vsFilename, polygonLayout, layoutCount);
	if(!result)
		return false;

	return true;
}

void VertexShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	ShutdownShader();

	return;
}

bool VertexShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, const XMFLOAT4X4 &worldMatrix, const XMFLOAT4X4 &viewMatrix, const XMFLOAT4X4 &projectionMatrix, 
						 ID3D11ShaderResourceView* texture)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if(!result)
		return false;

	//Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool VertexShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;

	//Compile the vertex shader code
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "MyVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &vertexShaderBuffer, &errorMessage, NULL);

	if(FAILED(result))
	{
		//If the shader failed to compile it should have written something to the error message
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
		return false;


	//Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, layoutCount, vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
		return false;

	//Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
		return false;

	return true;
}

void VertexShaderClass::ShutdownShader()
{
	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void VertexShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool VertexShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix,
												ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	XMFLOAT4X4 transposedWorldFloatMatrix, transposedViewFloatMatrix, transposedProjectionFloatMatrix;
	XMMATRIX transposedWorldMatrix, transposedViewMatrix, transposedProjectionMatrix;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//Transpose the matrices to prepare them for the shader
	transposedWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix));
	transposedViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
	transposedProjectionMatrix = XMMatrixTranspose(XMLoadFloat4x4(&projectionMatrix));

	XMStoreFloat4x4(&transposedWorldFloatMatrix, transposedWorldMatrix);
	XMStoreFloat4x4(&transposedViewFloatMatrix, transposedViewMatrix);
	XMStoreFloat4x4(&transposedProjectionFloatMatrix, transposedProjectionMatrix);

	//Lock the constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	//Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//Copy the matrices into the constant buffer
	dataPtr->world = transposedWorldFloatMatrix;
	dataPtr->view = transposedViewFloatMatrix;
	dataPtr->projection = transposedProjectionFloatMatrix;

	//Unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Finally set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

void VertexShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);

	return ;
}