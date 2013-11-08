////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MeshClass.h"
#include "MaterialClass.h"
#include "TextureClass.h"
#include "ObjectMeshClass.h"
#include "MeshDataClass.h"
#include "WindowsHelpers.h"

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

	return true;
}

bool MeshClass::Initialize(ObjectMeshClass *object, MeshClass::MeshType type)
{
	//bool result;

	m_type = type;

	m_allObjects = vector<ObjectMeshClass*>();
	m_guns = vector<DirectX::XMFLOAT3>();

	m_allObjects.push_back(object);

	return true;
}

void MeshClass::Shutdown()
{
	ReleaseModel();

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

void MeshClass::ReleaseModel()
{
	vector<ObjectMeshClass*>::iterator it;
	for (it = m_allObjects.begin(); it != m_allObjects.end(); ++it)
	{
		(*it)->Shutdown();
	}

	return;
}

MeshClass::MeshType MeshClass::getMeshType()
{
	return m_type;
}