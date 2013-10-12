#include "ModelClass.h"
#include "MeshClass.h"
#include "MaterialClass.h"

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices;
	VertexTextureType* texVertices;
	VertexMaterialType* matVertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, colorBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, colorData;
	HRESULT result;
	int i;

	// Create the index array.
	indices = new unsigned long[m_mesh->getIndexCount()];
	if (!indices)
	{
		return false;
	}

	if (m_mesh->getMeshColorType() == MeshClass::MeshColorType::MATERIAL)
	{
		// Create the vertex array.
		matVertices = new VertexMaterialType[m_mesh->getVertexCount()];
		if (!matVertices)
		{
			return false;
		}

		// Load the vertex array and index array with data.
		for (i = 0; i < m_mesh->getVertexCount(); i++)
		{
			matVertices[i].position = XMFLOAT3(m_mesh->getMesh()[i].x, m_mesh->getMesh()[i].y, m_mesh->getMesh()[i].z);
			matVertices[i].normals = XMFLOAT3(m_mesh->getMesh()[i].nx, m_mesh->getMesh()[i].ny, m_mesh->getMesh()[i].nz);
			matVertices[i].color = XMFLOAT4(m_mesh->getMaterial()->getColors()[i].r, m_mesh->getMaterial()->getColors()[i].g, m_mesh->getMaterial()->getColors()[i].b, 1.0f);

			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexMaterialType) * m_mesh->getVertexCount();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = matVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
	}
	else if (m_mesh->getMeshColorType() == MeshClass::MeshColorType::TEXTURE)
	{
		// Create the vertex array.
		texVertices = new VertexTextureType[m_mesh->getVertexCount()];
		if (!texVertices)
		{
			return false;
		}

		// Load the vertex array and index array with data.
		for (i = 0; i < m_mesh->getVertexCount(); i++)
		{
			texVertices[i].position = XMFLOAT3(m_mesh->getMesh()[i].x, m_mesh->getMesh()[i].y, m_mesh->getMesh()[i].z);
			texVertices[i].texture = XMFLOAT2(m_mesh->getMesh()[i].tu, m_mesh->getMesh()[i].tv);
			texVertices[i].normals = XMFLOAT3(m_mesh->getMesh()[i].nx, m_mesh->getMesh()[i].ny, m_mesh->getMesh()[i].nz);

			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * m_mesh->getVertexCount();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = texVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
	}

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_mesh->getIndexCount();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	if (m_mesh->getMeshColorType() == MeshClass::MeshColorType::MATERIAL)
	{
		delete [] matVertices;
		matVertices = 0;
	}
	else if (m_mesh->getMeshColorType() == MeshClass::MeshColorType::TEXTURE)
	{
		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete [] texVertices;
		texVertices = 0;
	}

	delete [] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	// Release the color buffer.
	if (m_colorBuffer)
	{
		m_colorBuffer->Release();
		m_colorBuffer = 0;
	}

	return;
}