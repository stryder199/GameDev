#include "ModelClass.h"
#include "MeshClass.h"
#include "MeshDataClass.h"
#include "MaterialClass.h"
#include "ObjectMeshClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "ShaderControllerClass.h"
#include "TextureClass.h"

bool ModelClass::InitializeBuffers()
{
	// For each object in the mesh
	std::vector<ObjectMeshClass*>::iterator object;
	std::vector<ObjectMeshClass*>* allObject = m_mesh->getAllObjects();
	for (object = allObject->begin(); object != allObject->end(); ++object)
	{
		// For each submesh
		std::vector<MeshDataClass*>::iterator subMesh;
		std::vector<MeshDataClass*>* allSubmeshs = (*object)->getAllMeshData();
		for (subMesh = allSubmeshs->begin(); subMesh != allSubmeshs->end(); ++subMesh)
		{
			unsigned long* indices;
			VertexTextureType* texVertices;
			VertexMaterialType* matVertices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, colorBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData, colorData;
			ID3D11Buffer *vertexBuf, *indexBuf;
			HRESULT result;

			// Create the index array.
			indices = new unsigned long[(*subMesh)->getIndexCount()];
			if (!indices)
			{
				return false;
			}

			if ((*subMesh)->getMeshColorType() == MeshDataClass::MeshColorType::MATERIAL)
			{
				// Create the vertex array.
				matVertices = new VertexMaterialType[(*subMesh)->getVertexCount()];
				if (!matVertices)
				{
					return false;
				}

				// For each mesh data
				std::vector<MeshDataClass::MeshType>::iterator rawMeshData;
				std::vector<MeshDataClass::MeshType>* allMeshData = (*subMesh)->getRawMeshData();

				std::vector<MaterialClass::ColorType>* allColorData = (*subMesh)->getMaterial()->getColors();
				std::vector<MaterialClass::ColorType>::iterator colorData = allColorData->begin();

				int count = 0;
				// Load the vertex array and index array with data.
				for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
				{
					matVertices[count].position = XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
					matVertices[count].normals = XMFLOAT3((*rawMeshData).nx, (*rawMeshData).ny, (*rawMeshData).nz);
					matVertices[count].color = XMFLOAT4((*colorData).r, (*colorData).g, (*colorData).b, 1.0f);

					indices[count] = count;
					count++;
					colorData++;
				}

				// Set up the description of the static vertex buffer.
				vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufferDesc.ByteWidth = sizeof(VertexMaterialType) * (*subMesh)->getVertexCount();
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = 0;
				vertexBufferDesc.MiscFlags = 0;
				vertexBufferDesc.StructureByteStride = 0;

				// Give the subresource structure a pointer to the vertex data.
				vertexData.pSysMem = matVertices;
				vertexData.SysMemPitch = 0;
				vertexData.SysMemSlicePitch = 0;
			}
			else if ((*subMesh)->getMeshColorType() == MeshDataClass::MeshColorType::TEXTURE)
			{
				// Create the vertex array.
				texVertices = new VertexTextureType[(*subMesh)->getVertexCount()];
				if (!texVertices)
				{
					return false;
				}

				// For each mesh data
				std::vector<MeshDataClass::MeshType>::iterator rawMeshData;
				std::vector<MeshDataClass::MeshType>* allMeshData = (*subMesh)->getRawMeshData();
				int count = 0;
				// Load the vertex array and index array with data.
				for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
				{
					texVertices[count].position = XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
					texVertices[count].texture = XMFLOAT2((*rawMeshData).tu, (*rawMeshData).tv);
					texVertices[count].normals = XMFLOAT3((*rawMeshData).nx, (*rawMeshData).ny, (*rawMeshData).nz);

					indices[count] = count;
					count++;
				}

				// Set up the description of the static vertex buffer.
				vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * (*subMesh)->getVertexCount();
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
			result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuf);
			if (FAILED(result))
			{
				return false;
			}

			(*subMesh)->setVertexBuffer(vertexBuf);

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*subMesh)->getIndexCount();
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuf);
			if (FAILED(result))
			{
				return false;
			}

			(*subMesh)->setIndexBuffer(indexBuf);

			if ((*subMesh)->getMeshColorType() == MeshDataClass::MeshColorType::MATERIAL)
			{
				delete[] matVertices;
				matVertices = 0;
			}
			else if ((*subMesh)->getMeshColorType() == MeshDataClass::MeshColorType::TEXTURE)
			{
				// Release the arrays now that the vertex and index buffers have been created and loaded.
				delete[] texVertices;
				texVertices = 0;
			}

			delete[] indices;
			indices = 0;
		}
	}

	return true;
}

bool ModelClass::RenderBuffers(ShaderControllerClass* shader, CameraClass* camera, LightClass* lightSource)
{
	bool result;
	XMFLOAT4X4 projMatrix;

	D3DClass::getInstance()->GetProjectionMatrix(projMatrix);

	// For each object in the mesh
	std::vector<ObjectMeshClass*>::iterator object;
	std::vector<ObjectMeshClass*>* allObject = m_mesh->getAllObjects();
	for (object = allObject->begin(); object != allObject->end(); ++object)
	{
		// For each submesh
		std::vector<MeshDataClass*>::iterator subMesh;
		std::vector<MeshDataClass*>* allMeshData = (*object)->getAllMeshData();
		for (subMesh = allMeshData->begin(); subMesh != allMeshData->end(); ++subMesh)
		{
			unsigned int vertexstride;
			unsigned int offset;

			//Set vertex buffer stride and offset.
			if ((*subMesh)->getMeshColorType() == MeshDataClass::MeshColorType::MATERIAL)
			{
				vertexstride = sizeof(VertexMaterialType);
				shader->Set3DMaterialShaders();
			}
			else if ((*subMesh)->getMeshColorType() == MeshDataClass::MeshColorType::TEXTURE)
			{
				vertexstride = sizeof(VertexTextureType);
				shader->Set3DTextureShaders();
			}
			offset = 0;

			ID3D11Buffer* tmp = (*subMesh)->getVertexBuffer();

			//Set the vertex buffer to active in the input assembler so it can be rendered.
			D3DClass::getInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &tmp, &vertexstride, &offset);

			//Set the index buffer to active in the input assembler so it can be rendered.
			D3DClass::getInstance()->GetDeviceContext()->IASetIndexBuffer((*subMesh)->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			D3DClass::getInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//Render the model using the shader
			result = shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, *camera->GetViewMatrix(), projMatrix,
				(*subMesh)->getTexture(), lightSource->GetDirection(), lightSource->GetAmbientColor(), lightSource->GetDiffuseColor());
			if (!result)
				return false;

		}
	}

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// For each object in the mesh
	std::vector<ObjectMeshClass*>::iterator object;
	std::vector<ObjectMeshClass*>* allObject = m_mesh->getAllObjects();
	for (object = allObject->begin(); object != allObject->end(); ++object)
	{
		// For each submesh
		std::vector<MeshDataClass*>::iterator subMesh;
		std::vector<MeshDataClass*>* allMeshData = (*object)->getAllMeshData();
		for (subMesh = allMeshData->begin(); subMesh != allMeshData->end(); ++subMesh)
		{
			// Release the index buffer.
			if ((*subMesh)->getIndexBuffer())
			{
				(*subMesh)->getIndexBuffer()->Release();
			}

			// Release the vertex buffer.
			if ((*subMesh)->getVertexBuffer())
			{
				(*subMesh)->getVertexBuffer()->Release();
			}

			// Release the color buffer.
			if ((*subMesh)->getColorBuffer())
			{
				(*subMesh)->getColorBuffer()->Release();

			}
		}
	}

	return;
}

void ModelClass::CalculateWorldMatrix()
{
	XMMATRIX worldMatrix = XMMatrixIdentity();

	// Move the model to the location it should be rendered at.
	XMMATRIX translationMatrix = XMMatrixTranslation(pos_x, pos_y, pos_z);
	XMMATRIX pointTranslationMatrix = XMMatrixTranslation(point_pos_x, point_pos_y, point_pos_z);
	XMMATRIX scalingMatrix = XMMatrixScaling(scale_x, scale_y, scale_z);
	XMMATRIX rotXMatrix = XMMatrixRotationX(rot_x);
	XMMATRIX rotYMatrix = XMMatrixRotationY(rot_y);
	XMMATRIX rotZMatrix = XMMatrixRotationZ(rot_z);


	worldMatrix *= scalingMatrix;
	worldMatrix *= pointTranslationMatrix;
	worldMatrix *= rotXMatrix;
	worldMatrix *= rotYMatrix;
	worldMatrix *= rotZMatrix;
	worldMatrix *= translationMatrix;

	XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
}

float ModelClass::getPositionX()
{
	return pos_x;
}

float ModelClass::getPositionY()
{
	return pos_y;
}

float ModelClass::getPositionZ()
{
	return pos_z;
}

float ModelClass::getRotationX()
{
	return rot_x;
}

float ModelClass::getRotationY()
{
	return rot_y;
}

float ModelClass::getRotationZ()
{
	return rot_z;
}

float ModelClass::getScaleX()
{
	return scale_x;
}

float ModelClass::getScaleY()
{
	return scale_y;
}

float ModelClass::getScaleZ()
{
	return scale_z;
}

float ModelClass::getPointPositionX()
{
	return point_pos_x;
}

float ModelClass::getPointPositionY()
{
	return point_pos_y;
}

float ModelClass::getPointPositionZ()
{
	return point_pos_z;
}