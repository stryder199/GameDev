////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MeshClass.h"
#include "MaterialClass.h"
#include "TextureClass.h"
#include "ObjectMeshClass.h"
#include "MeshDataClass.h"
#include "WindowsHelpers.h"
#include "D3DClass.h"

string readStringUntilSpace(string::iterator* it)
{
	string finalString = "";
	while (*(*it) != ' ')
	{
		finalString += *(*it);
		++*it;
	}

	//Go past the space or \n
	++*it;
	return finalString;
}

string readStringUntilNewL(string::iterator* it)
{
	string finalString = "";
	while (*(*it) != '\n' && *(*it) != '\r')
	{
		finalString += *(*it);
		++*it;
	}
	
	//Go past \n and \r
	if (*(*it) == '\r')
	{
		++*it;
	}
	
	++*it;
	return finalString;
}

float readFloat(string::iterator* it)
{
	string finalString = "";
	while (*(*it) != ' ' && *(*it) != '\n' &&  *(*it) != '\r')
	{
		finalString += *(*it);
		++*it;
	}

	if (*(*it) == '\r')
	{
		++*it;
	}
	//Go past the space or \n
	++*it;
	return (float)atof(finalString.c_str());
}

MaterialClass::MaterialInfo readMtlLine(string::iterator* it)
{
	string sinput;
	// Load material Data
	MaterialClass::MaterialInfo colorInfo = MaterialClass::MaterialInfo();
	colorInfo.Ns = readFloat(it);
	colorInfo.Ka_r = readFloat(it);
	colorInfo.Ka_g = readFloat(it);
	colorInfo.Ka_b = readFloat(it);
	colorInfo.Kd_r = readFloat(it);
	colorInfo.Kd_g = readFloat(it);
	colorInfo.Kd_b = readFloat(it);
	colorInfo.Ks_r = readFloat(it);
	colorInfo.Ks_g = readFloat(it);
	colorInfo.Ks_b = readFloat(it);
	colorInfo.Ni = readFloat(it);
	colorInfo.d = readFloat(it);
	colorInfo.illum = readFloat(it);
	colorInfo.map_Kd = readStringUntilNewL(it);

	return colorInfo;
}

MeshDataClass::MeshDataType readVtnLine(string::iterator* it)
{
	//Load vertices until a new object starts
	MeshDataClass::MeshDataType newMeshData = MeshDataClass::MeshDataType();

	newMeshData.x = readFloat(it);
	newMeshData.y = readFloat(it);
	newMeshData.z = readFloat(it);
	newMeshData.tu = readFloat(it);
	newMeshData.tv = readFloat(it);
	newMeshData.nx = readFloat(it);
	newMeshData.ny = readFloat(it);
	newMeshData.nz = readFloat(it);

	return newMeshData;
}

DirectX::XMFLOAT3 readGunLine(string::iterator* it)
{
	//Load vertices until a new object starts
	DirectX::XMFLOAT3 newVertex = DirectX::XMFLOAT3();

	newVertex.x = readFloat(it);
	newVertex.y = readFloat(it);
	newVertex.z = readFloat(it);

	return newVertex;
}

bool MeshClass::Initialize(string meshFilename, MeshClass::MeshType type)
{
	bool result;

	m_type = type;

	m_allObjects = vector<ObjectMeshClass*>();
	m_guns = vector<DirectX::XMFLOAT3>();

	// Load in the model data,
	result = LoadModel(meshFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers();
	if (!result)
		return false;

	return true;
}

bool MeshClass::Initialize(ObjectMeshClass *object, MeshClass::MeshType type)
{
	bool result;

	m_type = type;

	m_allObjects = vector<ObjectMeshClass*>();
	m_guns = vector<DirectX::XMFLOAT3>();

	m_allObjects.push_back(object);

	result = InitializeBuffers();
	if (!result)
		return false;

	return true;
}

bool MeshClass::InitializeBuffers()
{
	// For each object in the mesh
	std::vector<ObjectMeshClass*>::iterator object;
	for (object = m_allObjects.begin(); object != m_allObjects.end(); ++object)
	{
		// For each submesh
		std::vector<MeshDataClass*>::iterator subMesh;
		std::vector<MeshDataClass*>* allSubmeshs = (*object)->getAllMeshData();
		for (subMesh = allSubmeshs->begin(); subMesh != allSubmeshs->end(); ++subMesh)
		{
			unsigned long* indices;
			VertexTextureType* texVertices;
			VertexMaterialType* matVertices;
			VertexTextType* textVertices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			ID3D11Buffer *vertexBuf, *indexBuf;
			HRESULT result;

			// Create the index array.
			indices = new unsigned long[(*subMesh)->getIndexCount()];
			if (!indices)
			{
				return false;
			}

			if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
			{
				// Create the vertex array.
				matVertices = new VertexMaterialType[(*subMesh)->getVertexCount()];
				if (!matVertices)
				{
					return false;
				}

				// For each mesh data
				std::vector<MeshDataClass::MeshDataType>::iterator rawMeshData;
				std::vector<MeshDataClass::MeshDataType>* allMeshData = (*subMesh)->getRawMeshData();

				int count = 0;
				// Load the vertex array and index array with data.
				for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
				{
					matVertices[count].position = DirectX::XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
					matVertices[count].normals = DirectX::XMFLOAT3((*rawMeshData).nx, (*rawMeshData).ny, (*rawMeshData).nz);

					indices[count] = count;
					count++;
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
			else if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
			{
				// Create the vertex array.
				texVertices = new VertexTextureType[(*subMesh)->getVertexCount()];
				if (!texVertices)
				{
					return false;
				}

				// For each mesh data
				std::vector<MeshDataClass::MeshDataType>::iterator rawMeshData;
				std::vector<MeshDataClass::MeshDataType>* allMeshData = (*subMesh)->getRawMeshData();
				int count = 0;
				// Load the vertex array and index array with data.
				for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
				{
					texVertices[count].position = DirectX::XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
					texVertices[count].texture = DirectX::XMFLOAT2((*rawMeshData).tu, (*rawMeshData).tv);
					texVertices[count].normals = DirectX::XMFLOAT3((*rawMeshData).nx, (*rawMeshData).ny, (*rawMeshData).nz);

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
			else if (m_type == MeshClass::TEXT || m_type == MeshClass::TWOD)
			{
				// Create the vertex array.
				textVertices = new VertexTextType[(*subMesh)->getVertexCount()];
				if (!textVertices)
				{
					return false;
				}

				// For each mesh data
				std::vector<MeshDataClass::MeshDataType>::iterator rawMeshData;
				std::vector<MeshDataClass::MeshDataType>* allMeshData = (*subMesh)->getRawMeshData();
				int count = 0;
				// Load the vertex array and index array with data.
				for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
				{
					textVertices[count].position = DirectX::XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
					textVertices[count].texture = DirectX::XMFLOAT2((*rawMeshData).tu, (*rawMeshData).tv);

					indices[count] = count;
					count++;
				}

				// Set up the description of the static vertex buffer.
				vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufferDesc.ByteWidth = sizeof(VertexTextType) * (*subMesh)->getVertexCount();
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = 0;
				vertexBufferDesc.MiscFlags = 0;
				vertexBufferDesc.StructureByteStride = 0;

				// Give the subresource structure a pointer to the vertex data.
				vertexData.pSysMem = textVertices;
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

			if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
			{
				delete[] matVertices;
				matVertices = 0;
			}
			else if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
			{
				delete[] texVertices;
				texVertices = 0;
			}
			else if (m_type == MeshClass::TEXT || m_type == MeshClass::TWOD)
			{
				delete[] textVertices;
				textVertices = 0;
			}

			delete[] indices;
			indices = 0;
		}
	}

	return true;
}

void MeshClass::Shutdown()
{
	vector<ObjectMeshClass*>::iterator it;
	for (it = m_allObjects.begin(); it != m_allObjects.end(); ++it)
	{
		(*it)->Shutdown();
	}

	return;
}

bool MeshClass::LoadModel(string filename)
{
	string sinput;
	bool result;
	
	string s;

	ifstream is(filename, ifstream::binary);
	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		int length = (int)is.tellg();
		is.seekg(0, is.beg);

		char * buffer = new char[length];
	
		// read data as a block:
		is.read(buffer, length);

		
		s.reserve(sizeof(char) *length);
		s = string(buffer);

		// ...buffer contains the entire file...

		delete[] buffer;
	}
	else {
		cerr << "Could not find file: " << filename << endl;
		return false;
	}
	is.close();

	string::iterator it = s.begin();

	MaterialClass* focusMaterial = 0;
	TextureClass* focusTexture = 0;
	ObjectMeshClass* focusObject = 0;
	MeshDataClass* focusMesh = 0;
	MeshDataClass::MeshColorType focusType;

	// For each object in the model
	while ((*it) != 'e')
	{
		//get next command
		sinput = readStringUntilSpace(&it);

		if (sinput.compare("vtn") == 0)
		{
			//Load vertices until a new object starts
			MeshDataClass::MeshDataType newMeshData = readVtnLine(&it);
			focusMesh->addMeshData(newMeshData);
			
			//Should auto skip the new line
			if ((*it) == 'm' || (*it) == 'o')
			{
				//Push the last mesh
				focusObject->addMesh(focusMesh);

			}
		}
		else if (sinput.compare("o") == 0)
		{
			//If your not the first object
			if (focusObject != 0)
			{
				//Push the last object
				m_allObjects.push_back(focusObject);
			}
			string objectName = readStringUntilNewL(&it);

			focusObject = new ObjectMeshClass();
			result = focusObject->Initialize(objectName);
			if (!result)
				return false;

			//Should auto skip to the next line
			
			//Check if you need to use the prev material
			if ((*it) == 'v')
			{
				focusMesh = new MeshDataClass();

				if (focusType == MeshDataClass::MeshColorType::MATERIAL)
				{
					result = focusMesh->Initialize(focusMaterial);
					if (!result)
						return false;
				}
				else
				{
					result = focusMesh->Initialize(focusTexture);
					if (!result)
						return false;
				}
			}
		}
		else if (sinput.compare("mtl") == 0)
		{
			focusMesh = new MeshDataClass();
			MaterialClass::MaterialInfo focusColorInfo = MaterialClass::MaterialInfo();

			focusColorInfo = readMtlLine(&it);

			if (focusColorInfo.map_Kd.compare("material") == 0)
			{
				MaterialClass* material = new MaterialClass();
				result = material->Initialize(focusColorInfo);
				if (!result)
					return false;

				focusType = MeshDataClass::MeshColorType::MATERIAL;

				result = focusMesh->Initialize(material);
				if (!result)
					return false;
			}
			else
			{
				TextureClass* texture = new TextureClass();
				result = texture->Initialize(toWChar("data\\" + focusColorInfo.map_Kd));
				if (!result)
					return false;

				focusTexture = texture;
				focusType = MeshDataClass::MeshColorType::TEXTURE;

				result = focusMesh->Initialize(texture);
				if (!result)
					return false;
			}

			//Should autoskip to the next line
		}	
		else if (sinput.compare("gun") == 0)
		{
			DirectX::XMFLOAT3 newGun = readGunLine(&it);
			m_guns.push_back(newGun);
		}
	}

	//Add the last objects
	focusObject->addMesh(focusMesh);
	m_allObjects.push_back(focusObject);

	return true;
}

vector<ObjectMeshClass*>* MeshClass::getAllObjects()
{
	return &m_allObjects;
}

vector<DirectX::XMFLOAT3>* MeshClass::getGuns()
{
	return &m_guns;
}

MeshClass::MeshType MeshClass::getMeshType()
{
	return m_type;
}