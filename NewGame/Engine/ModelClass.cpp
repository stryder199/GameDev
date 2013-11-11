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
#include "PixelShaderClass.h"

bool ModelClass::RenderBuffers(ShaderControllerClass* shader)
{
	bool result;

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

			if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
			{
				vertexstride = sizeof(MeshClass::VertexMaterialType);
				shader->Set3DMaterialShaders();
			}
			else if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
			{
				vertexstride = sizeof(MeshClass::VertexTextureType);
				shader->Set3DTextureShaders();
			}
			else if (m_mesh->getMeshType() == MeshClass::TEXT || m_mesh->getMeshType() == MeshClass::TWOD)
			{
				vertexstride = sizeof(MeshClass::VertexTextType);
			}
			else
			{
				return false;
			}

			offset = 0;

			ID3D11Buffer* tmp = (*subMesh)->getVertexBuffer();

			//Set the vertex buffer to active in the input assembler so it can be rendered.
			D3DClass::getInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &tmp, &vertexstride, &offset);

			//Set the index buffer to active in the input assembler so it can be rendered.
			D3DClass::getInstance()->GetDeviceContext()->IASetIndexBuffer((*subMesh)->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			D3DClass::getInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
			{
				XMFLOAT4 color = XMFLOAT4((*subMesh)->getMaterial()->getMaterialInfo().Kd_r,
					(*subMesh)->getMaterial()->getMaterialInfo().Kd_g,
					(*subMesh)->getMaterial()->getMaterialInfo().Kd_b,
					1.0f);
				//Render the model using the shader
				result = shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, m_lightSource->GetDirection(), 
					m_lightSource->GetAmbientColor(), m_lightSource->GetDiffuseColor(), color);
				if (!result)
					return false;
			}
			else if (m_mesh->getMeshType() == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
			{
				//Render the model using the shader
				result = shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, (*subMesh)->getTexture(), m_lightSource->GetDirection(),
					m_lightSource->GetAmbientColor(), m_lightSource->GetDiffuseColor());
				if (!result)
					return false;
			}
			else if (m_mesh->getMeshType() == MeshClass::TEXT)
			{
				//Render the model using the shader
				result = shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, (*subMesh)->getTexture(), m_color);
				if (!result)
					return false;
			}
			else if (m_mesh->getMeshType() == MeshClass::TWOD)
			{
				//Render the model using the shader
				result = shader->Render((*subMesh)->getIndexCount(), m_worldMatrix, (*subMesh)->getTexture());
				if (!result)
					return false;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

void ModelClass::CalculateWorldMatrix()
{
	XMMATRIX worldMatrix = XMMatrixIdentity();

	// Move the model to the location it should be rendered at.
	XMMATRIX translationMatrix = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	XMMATRIX pointTranslationMatrix = XMMatrixTranslation(m_point_pos.x, m_point_pos.y, m_point_pos.z);
	XMMATRIX scalingMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX rotXMatrix = XMMatrixRotationX(m_rot.x);
	XMMATRIX rotYMatrix = XMMatrixRotationY(m_rot.y);
	XMMATRIX rotZMatrix = XMMatrixRotationZ(m_rot.z);


	worldMatrix *= scalingMatrix;
	worldMatrix *= pointTranslationMatrix;
	worldMatrix *= rotXMatrix;
	worldMatrix *= rotYMatrix;
	worldMatrix *= rotZMatrix;
	worldMatrix *= translationMatrix;

	XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
}

XMFLOAT3 ModelClass::getPosition()
{
	return m_pos;
}

XMFLOAT3 ModelClass::getRotation()
{
	return m_rot;
}

XMFLOAT3 ModelClass::getScale()
{
	return m_scale;
}

XMFLOAT3 ModelClass::getPointPosition()
{
	return m_point_pos;
}

XMFLOAT3 ModelClass::getDirection()
{
	return m_dir;
}

void ModelClass::ConstrainRotation()
{
	if (m_rot.x >= 2 * (float) XM_PI)
		m_rot.x = m_rot.x - 2 * (float) XM_PI;
	else if (m_rot.x < 0.0f)
		m_rot.x = 2 * (float) XM_PI + m_rot.x;

	if (m_rot.y >= 2 * (float) XM_PI)
		m_rot.y = m_rot.y - 2 * (float) XM_PI;
	else if (m_rot.y < 0.0f)
		m_rot.y = 2 * (float) XM_PI + m_rot.y;

	if (m_rot.z >= 2 * (float) XM_PI)
		m_rot.z = m_rot.z - 2 * (float) XM_PI;
	else if (m_rot.z < 0.0f)
		m_rot.z = 2 * (float) XM_PI + m_rot.z;
}

void ModelClass::CalculateDirection()
{
	if (m_rot.y >= 0.0f && m_rot.y < (XM_PI / 2.0f))
	{
		m_dir.x = (m_rot.y / (XM_PI / 2.0f));
	}
	else if (m_rot.y >= (XM_PI / 2.0f) && m_rot.y < ((3.0f*XM_PI) / 2.0f))
	{
		m_dir.x = ((XM_PI - m_rot.y) / (XM_PI / 2.0f));
	}
	else if (m_rot.y >= ((3.0f*XM_PI) / 2.0f) && m_rot.y < (2 * XM_PI))
	{
		m_dir.x = ((m_rot.y - 2 * XM_PI) / (XM_PI / 2.0f));
	}

	if (m_rot.y >= 0.0f && m_rot.y < XM_PI)
	{
		m_dir.z = (((XM_PI / 2.0f) - m_rot.y) / (XM_PI / 2.0f));
	}
	else if (m_rot.y >= XM_PI && m_rot.y < (2 * XM_PI))
	{
		m_dir.z = ((m_rot.y - ((3.0f*XM_PI) / 2.0f)) / (XM_PI / 2.0f));
	}
	XMVECTOR normalizedVector;
	normalizedVector = XMVector3Normalize(XMLoadFloat3(&m_dir));
	XMStoreFloat3(&m_dir, normalizedVector);
}