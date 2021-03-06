#include "VertexShaderClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "WindowClass.h"
#include "GenericException.h"
#include "D3DX11.h"
#include <fstream>

using namespace std;
using namespace DirectX;

VertexShaderClass::VertexShaderClass()
{
    m_vertexShader = nullptr;
    m_layout = nullptr;
    m_matrixBuffer = nullptr;
}

VertexShaderClass::~VertexShaderClass()
{
}

void VertexShaderClass::Initialize(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount, VertexShaderClass::ShaderType type)
{
    m_type = type;

    //Initialize the vertex and pixel shaders
    InitializeShader(vsFilename, polygonLayout, layoutCount);
}

void VertexShaderClass::Shutdown()
{
    //Shutdown the vertex and pixel shaders as well as the related objects
    ShutdownShader();

    return;
}

void VertexShaderClass::Render(int indexCount, const XMFLOAT4X4 &worldMatrix)
{
    //Set the shader parameters that it will use for rendering
    SetShaderParameters(worldMatrix);

    //Now render the prepared buffers with the shader.
    RenderShader(indexCount);
}

void VertexShaderClass::InitializeShader(WCHAR* vsFilename, D3D11_INPUT_ELEMENT_DESC* polygonLayout, int layoutCount)
{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    D3D11_BUFFER_DESC matrixBufferDesc;


    // Initialize the pointers this function will use to nullptr.
    errorMessage = 0;
    vertexShaderBuffer = 0;

    //Compile the vertex shader code
    result = D3DX11CompileFromFile(vsFilename, nullptr, nullptr, "MyVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, 
                       &vertexShaderBuffer, &errorMessage, nullptr);

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
        throw GenericException("Failed to compile the vertex shader.");
    }

    //Create the vertex shader from the buffer
    result = D3DClass::getInstance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
    if (FAILED(result))
    {
        throw GenericException("Failed to create the vertex shader.");
    }


    //Create the vertex input layout
    result = D3DClass::getInstance()->GetDevice()->CreateInputLayout(polygonLayout, layoutCount, vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), &m_layout);
    if(FAILED(result))
    {
        throw GenericException("Failed to create the vertex input layout.");
    }

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
    result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
    if(FAILED(result))
    {
        throw GenericException("Failed to create the constant buffer.");
    }
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
    ofstream fout;


    // Get a pointer to the error message text buffer.
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // Get the length of the message.
    bufferSize = errorMessage->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader-error.txt");
    if (fout.fail())
    {
        throw GenericException("Failed to open the file");
    }

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
}

void VertexShaderClass::SetShaderParameters(const XMFLOAT4X4& worldMatrix)
{
    HRESULT result;
    XMFLOAT4X4 transposedWorldFloatMatrix, transposedViewFloatMatrix, transposedProjectionFloatMatrix;
    XMMATRIX transposedWorldMatrix, transposedViewMatrix, transposedProjectionMatrix;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;

    XMFLOAT4X4 viewMatrix = CameraClass::getInstance()->GetViewMatrix();

    //Transpose the matrices to prepare them for the shader
    transposedWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix));
    transposedViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));

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
        throw GenericException("Unknown texture type.");
    }

    XMStoreFloat4x4(&transposedWorldFloatMatrix, transposedWorldMatrix);
    XMStoreFloat4x4(&transposedViewFloatMatrix, transposedViewMatrix);
    XMStoreFloat4x4(&transposedProjectionFloatMatrix, transposedProjectionMatrix);

    //Lock the constant buffer so it can be written to
    result = D3DClass::getInstance()->GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(FAILED(result))
    {
        throw GenericException("Failed to lock the constant buffer.");
    }

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
}

void VertexShaderClass::RenderShader(int indexCount)
{
    //Set the vertex input layout
    D3DClass::getInstance()->GetDeviceContext()->IASetInputLayout(m_layout);

    //Set the vertex and pixel shaders that will be used to render this triangle
    D3DClass::getInstance()->GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
}