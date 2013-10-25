////////////////////////////////////////////////////////////////////////////////
// Filename: MeshClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MeshClass.h"
#include "MaterialClass.h"
#include "TextureClass.h"
#include "ObjectMeshClass.h"
#include "MeshDataClass.h"
#include "WindowsHelpers.h"

MeshClass::MeshClass()
{
}

MeshClass::MeshClass(const MeshClass& other)
{
}

MeshClass::~MeshClass()
{
}

MaterialClass::MaterialInfo readMtlLine(ifstream* fin)
{
	string sinput;
	// Load material Data
	MaterialClass::MaterialInfo colorInfo = MaterialClass::MaterialInfo();
	*fin >> colorInfo.Ns >> colorInfo.Ka_r >> colorInfo.Ka_g >> colorInfo.Ka_b >> colorInfo.Kd_r >>
		colorInfo.Kd_g >> colorInfo.Kd_b >> colorInfo.Ks_r >> colorInfo.Ks_g >> colorInfo.Ks_b >>
		colorInfo.Ni >> colorInfo.d >> colorInfo.illum;
	*fin >> sinput;
	colorInfo.map_Kd = sinput;

	while ((fin->peek() != '\n'))
	{
		*fin >> sinput;
		colorInfo.map_Kd = colorInfo.map_Kd + " " + sinput;
	}

	return colorInfo;
}

MeshDataClass::MeshType readVtnLine(ifstream* fin)
{
	//Load vertices until a new object starts
	MeshDataClass::MeshType newMeshData = MeshDataClass::MeshType();

	*fin >> newMeshData.x >> newMeshData.y >> newMeshData.z;
	*fin >> newMeshData.tu >> newMeshData.tv;
	*fin >> newMeshData.nx >> newMeshData.ny >> newMeshData.nz;

	return newMeshData;
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

	fin >> sinput;
	fin >> sinput;

	MaterialClass* currentMaterial = 0;
	TextureClass* currentTexture = 0;
	MeshDataClass::MeshColorType type = MeshDataClass::MeshColorType();

	// For each object in the model
	while (sinput != "")
	{
		if (sinput.compare("o") == 0)
		{
			//Load the object name
			fin >> sinput;
			string objectName = sinput;
			while (fin.peek() != '\n')
			{
				fin >> sinput;
				objectName = objectName + " " + sinput;
			}

			ObjectMeshClass* newObject = new ObjectMeshClass();
			result = newObject->Initialize(objectName);
			if (!result)
				return false;

			//Get the next command
			fin >> sinput;

			MeshDataClass* newMesh = new MeshDataClass();
			MaterialClass::MaterialInfo colorInfo = MaterialClass::MaterialInfo();
			
			//Are we using the same material as the last object??
			//If we have a material
			if (sinput.compare("mtl") == 0)
			{	
				colorInfo = readMtlLine(&fin);

				if (colorInfo.map_Kd.compare("material") == 0)
				{
					MaterialClass* material = new MaterialClass();
					result = material->Initialize(colorInfo);
					if (!result)
						return false;

					currentMaterial = material;
					type = MeshDataClass::MeshColorType::MATERIAL;

					result = newMesh->Initialize(material);
					if (!result)
						return false;
				}
				else
				{

					TextureClass* texture = new TextureClass();
					result = texture->Initialize(toWChar("data\\" + colorInfo.map_Kd));
					if (!result)
						return false;

					currentTexture = texture;
					type = MeshDataClass::MeshColorType::TEXTURE;

					result = newMesh->Initialize(texture);
					if (!result)
						return false;
				}

				//Get the next command
				fin >> sinput;
			}
			//If we want to use the same material as last time
			else
			{
				if (type == MeshDataClass::MeshColorType::TEXTURE)
				{
					result = newMesh->Initialize(currentTexture);
					if (!result)
						return false;
				}
				else if (type == MeshDataClass::MeshColorType::MATERIAL)
				{
					result = newMesh->Initialize(currentMaterial);
					if (!result)
						return false;
				}

				//You are already at the next command so dont get it
			}

			//For each material in the object
			while (sinput.compare("o") != 0 && sinput != "")
			{
				//For each vtn until you hit a new mtl or new o
				while (sinput.compare("mtl") != 0 && sinput.compare("o") != 0)
				{
					if (type == MeshDataClass::MeshColorType::MATERIAL)
					{
						MaterialClass::ColorType color;
						color.r = colorInfo.Kd_r;
						color.g = colorInfo.Kd_g;
						color.b = colorInfo.Kd_b;
						color.a = 1.0f;

						newMesh->getMaterial()->addColorData(color);
					}

					//Load vertices until a new object starts
					MeshDataClass::MeshType newMeshData = readVtnLine(&fin);
					newMesh->addMeshData(newMeshData);

					if (!(fin >> sinput))
					{
						sinput = "";
						break;
					}
						
				}

				// We are at the end of either our object or our material
				newObject->addMesh(newMesh);

				//If we have a material
				if (sinput.compare("mtl") == 0)
				{
					newMesh = new MeshDataClass();
					colorInfo = readMtlLine(&fin);

					if (colorInfo.map_Kd == "material")
					{
						MaterialClass* material = new MaterialClass();
						result = material->Initialize(colorInfo);
						if (!result)
							return false;

						currentMaterial = material;
						type = MeshDataClass::MeshColorType::MATERIAL;

						result = newMesh->Initialize(material);
						if (!result)
							return false;
					}
					else
					{

						TextureClass* texture = new TextureClass();
						result = texture->Initialize(toWChar("data\\" + colorInfo.map_Kd));
						if (!result)
							return false;

						currentTexture = texture;
						type = MeshDataClass::MeshColorType::TEXTURE;

						result = newMesh->Initialize(texture);
						if (!result)
							return false;
					}

					//Get the next command
					fin >> sinput;
				}
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