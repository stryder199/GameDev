////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MeshClass.h"
#include "MaterialClass.h"
#include "ObjectMeshClass.h"
#include "MeshDataClass.h"

MeshClass::MeshClass()
{
}

MeshClass::MeshClass(const MeshClass& other)
{
}

MeshClass::~MeshClass()
{
}

bool MeshClass::Initialize(char* meshFilename)
{
	bool result;

	m_allObjects = vector<ObjectMeshClass*>();

	// Load in the model data,
	result = LoadModel(meshFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void MeshClass::Shutdown()
{
	ReleaseModel();

	return;
}

bool MeshClass::LoadModel(char* filename)
{
	std::ifstream fin;
	std::string sinput;
	bool result;
	char input;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
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
	fin >> m_totalVertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_totalIndexCount = m_totalVertexCount;

	while (sinput != "")
	{
		if (sinput.compare("o") == 0)
		{
			ObjectMeshClass* newObject = new ObjectMeshClass();

			//Load the object name
			fin >> sinput;
			string objectName = sinput;
			while ((fin.peek() != '\n'))
			{
				fin >> sinput;
				objectName = objectName + " " + sinput;
			}

			result = newObject->Initialize(objectName);
			if (!result)
				return false;

			//Get the next command
			fin >> sinput;

			//If you are in the same object
			while (sinput.compare("o") != 0)
			{
				MeshDataClass::MeshColorType type;
				MeshDataClass* newMesh = new MeshDataClass();
				string textureFilename;

				// Load material Data
				MaterialClass::MaterialInfo colorInfo;
				fin >> colorInfo.Ns >> colorInfo.Ka_r >> colorInfo.Ka_g >> colorInfo.Ka_b >> colorInfo.Kd_r >>
					colorInfo.Kd_g >> colorInfo.Kd_b >> colorInfo.Ks_r >> colorInfo.Ks_g >> colorInfo.Ks_b >>
					colorInfo.Ni >> colorInfo.d >> colorInfo.illum;
				fin >> sinput;
				colorInfo.map_Kd = sinput;

				while ((fin.peek() != '\n'))
				{
					fin >> sinput;
					colorInfo.map_Kd = colorInfo.map_Kd + " " + sinput;
				}

				if (colorInfo.map_Kd == "material")
				{
					type = MeshDataClass::MeshColorType::MATERIAL;
					textureFilename = "";
				}
				else
				{
					type = MeshDataClass::MeshColorType::TEXTURE;
					textureFilename = colorInfo.map_Kd;
				}

				result = newMesh->Initialize(type, textureFilename);
				if (!result)
					return false;

				//Get the next command
				fin >> sinput;

				//If you are in the same object and material
				while (sinput.compare("newmtl") != 0 && sinput.compare("o") != 0)
				{
					//Load vertices until a new object starts
					MeshDataClass::MeshType newMeshData = MeshDataClass::MeshType();

					if (type == MeshDataClass::MeshColorType::MATERIAL)
					{
						MaterialClass::ColorType color;
						color.r = colorInfo.Kd_r;
						color.g = colorInfo.Kd_g;
						color.b = colorInfo.Kd_b;
						color.a = 1.0f;

						newMesh->getMaterial()->addColorData(color);
					}

					fin >> newMeshData.x >> newMeshData.y >> newMeshData.z;
					fin >> newMeshData.tu >> newMeshData.tv;
					fin >> newMeshData.nx >> newMeshData.ny >> newMeshData.nz;

					newMesh->addMeshData(newMeshData);

					fin >> sinput;
				}
				// We are at the end of either our object or our material
				newObject->addMesh(newMesh);
			}
			//We are indeed at the end of our object
			m_allObjects.push_back(newObject);
		}
	}

	// Close the model file.
	fin.close();

	return true;
}

vector<ObjectMeshClass*>* MeshClass::getAllObjects()
{
	return &m_allObjects;
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