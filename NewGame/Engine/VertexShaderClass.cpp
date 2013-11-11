////////////////////////////////////////////////////////////////////////////////
// Filename: VertexShaderClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "VertexShaderClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "WindowClass.h"

VertexShaderClass::VertexShaderClass()
{
	m_vertexShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

VertexShaderClass::~VertexShaderClass()
{
}

bool VertexShaderClass::Initialize(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount, VertexShaderClass::ShaderType type)
{
	bool result;

	m_type = type;

	//Initialize the vertex and pixel shaders
	result = InitializeShader(vsFilename, polygonLayout, layoutCount);
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

bool VertexShaderClass::Render(int indexCount, const XMFLOAT4X4 &worldMatrix)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(worldMatrix);
	if(!result)
		return false;

	//Now render the prepared buffers with the shader.
	RenderShader(indexCount);

	return true;
}

bool VertexShaderClass::InitializeShader(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount)
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
			OutputShaderErrorMessage(errorMessage, vsFilename);
		}
		else
		{
			MessageBox(WindowClass::getInstance()->gethWnd(), vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Create the vertex shader from the buffer
	result = D3DClass::getInstance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
		return false;


	//Create the vertex input layout
	result = D3DClass::getInstance()->GetDevice()->CreateInputLayout(polygonLayout, layoutCount, vertexShaderBuffer->GetBufferPointer(),
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
	result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
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

void VertexShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
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

bool VertexShaderClass::SetShaderParameters(const XMFLOAT4X4& worldMatrix)
{
	HRESULT result;
	XMFLOAT4X4 transposedWorldFloatMatrix, transposedViewFloatMatrix, transposedProjectionFloatMatrix;
	XMMATRIX transposedWorldMatrix, transposedViewMatrix, transposedProjectionMatrix;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//Transpose the matrices to prepare them for the shader
	transposedWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix));
	transposedViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(CameraClass::getInstance()->GetViewMatrix()));

	if (m_type == THREEDTEXTURE || m_type == THREEDMATERIAL)
	{
		transposedProjectionMatrix = XMMatrixTranspose(XMLoadFloat4x4(&D3DClass::getInstance()->GetProjectionMatrix()));
	}
	else if (m_type == TWOD || m_type == TEXT)
	{
		transposedProjectionMatrix = XMMatrixTranspose(XMLoadFloat4x4(&D3DClass::getInstance()->GetOrthoMatrix()));
	}
	else
	{
		return false;
	}

	XMStoreFloat4x4(&transposedWorldFloatMatrix, transposedWorldMatrix);
	XMStoreFloat4x4(&transposedViewFloatMatrix, transposedViewMatrix);
	XMStoreFloat4x4(&transposedProjectionFloatMatrix, transposedProjectionMatrix);

	//Lock the constant buffer so it can be written to
	result = D3DClass::getInstance()->GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	//Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//Copy the matrices into the constant buffer
	dataPtr->world = transposedWorldFloatMatrix;
	dataPtr->view = transposedViewFloatMatrix;
	dataPtr->projection = transposedProjectionFloatMatrix;

	//Unlock the constant buffer
	D3DClass::getInstance()->GetDeviceContext()->Unmap(m_matrixBuffer, 0);

	//Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Finally set the constant buffer in the vertex shader with the updated values
	D3DClass::getInstance()->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

void VertexShaderClass::RenderShader(int indexCount)
{
	//Set the vertex input layout
	D3DClass::getInstance()->GetDeviceContext()->IASetInputLayout(m_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	D3DClass::getInstance()->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);

	return ;
}