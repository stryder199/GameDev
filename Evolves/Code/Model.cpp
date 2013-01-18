#include "Model.h"
#include "Device.h"

struct SimpleVertex
{
    XMFLOAT3 Pos;
	XMFLOAT3 Norm;
	XMFLOAT4 Color;
};

struct ModelData
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

Model::Model()
{
	modelData = NULL;

	vertexCount = 0;
	indexCount = 0;

	//Set vertex buffer
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
}

Model::~Model()
{
	if(modelData)
	{
		delete [] modelData;
		modelData = 0;
	}
}


HRESULT Model::init(char *filename, Device *myDevice )
{
	HRESULT hr = S_OK;

	std::strcpy(dataFile, filename);

	hr = importMeshData();
	if( FAILED(hr) )
		return hr;

	hr = initBuffers( myDevice );
	if( FAILED(hr) )
		return hr;

	return S_OK;
}

HRESULT Model::importMeshData()
{
	std::ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(dataFile);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	// Create the model using the vertex count that was read in.
	modelData = new ModelData[vertexCount];
	if(!modelData)
	{
		return E_ABORT;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<vertexCount; i++)
	{
		fin >> modelData[i].x >> modelData[i].y >> modelData[i].z;
		fin >> modelData[i].tu >> modelData[i].tv;
		fin >> modelData[i].nx >> modelData[i].ny >> modelData[i].nz;
	}

	// Close the model file.
	fin.close();

	return S_OK;
}


HRESULT Model::initBuffers( Device *myDevice )
{
	HRESULT hr = S_OK;
	SimpleVertex* vertices;
	unsigned long* indices;

	// Create the vertex array.
	vertices = new SimpleVertex[vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(int i=0; i<vertexCount; i++)
	{
		vertices[i].Pos = XMFLOAT3(modelData[i].x, modelData[i].y, modelData[i].z);
		vertices[i].Norm = XMFLOAT3(modelData[i].nx, modelData[i].ny, modelData[i].nz);
		vertices[i].Color = XMFLOAT4( 0.0f, 0.0f, 1.0, 1.0f);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory( &vertexData, sizeof(vertexData) );
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = myDevice->g_pd3dDevice->CreateBuffer( &vertexBufferDesc, &vertexData, &g_pVertexBuffer );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the Index buffer
	hr = myDevice->g_pd3dDevice->CreateBuffer( &indexBufferDesc, &indexData, &g_pIndexBuffer );
	if( FAILED( hr ) )
	{
		return hr;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return S_OK;
}