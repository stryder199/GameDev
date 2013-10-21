#include "Exporter.h"


Exporter::Exporter()
{
	m_allMaterials = new map<string, ColorType>();
	m_materialFaceIndex = new map<int, string>();
	m_objectIndex = new map<int, string>();
	m_vertices = new vector<VertexType>();
	m_texcoords = new vector<VertexType>();
	m_normals = new vector<VertexType>();
	m_faces = new vector<FaceType>();;
}

Exporter::~Exporter()
{
}

bool Exporter::LoadDataStructures(string filename)
{
	bool result;
	string outputfilename, outputfilepath;

	result = LoadObjFileData(filename);
	if (!result)
	{
		return false;
	}

	outputfilename = filename;

	outputfilename.erase(outputfilename.end() - 4, outputfilename.end());

	outputfilename += ".3dmodel";

	outputfilepath = OUTPUT_FOLDER + "\\" + outputfilename;

	result = WriteOutputFile(outputfilepath);
	if (!result)
		return false;

	return true;
}

bool Exporter::LoadMaterialsFromFile(string mtlFilepath)
{
	string sinput = "";
	ifstream mtlin;

	// Read in the materials in the mtl file
	mtlin.open(mtlFilepath);

	// Check if it was successful in opening the file.
	if (mtlin.fail() == true)
	{
		cerr << "Could not find file: " + mtlFilepath << endl;
		return false;
	}

	while (!mtlin.eof())
	{
		ColorType newColor = {};
		string materialName;

		mtlin >> materialName;

		mtlin >> sinput;

		while (!mtlin.eof() && sinput != "newmtl")
		{
			if (sinput.compare("Ns") == 0)
			{
				mtlin >> newColor.Ns;
			}
			else if (sinput.compare("Ni") == 0)
			{
				mtlin >> newColor.Ni;
			}
			else if (sinput.compare("d") == 0)
			{
				mtlin >> newColor.d;
			}
			else if (sinput.compare("illum") == 0)
			{
				mtlin >> newColor.illum;
			}
			else if (sinput.compare("Ka") == 0)
			{
				mtlin >> newColor.Ka_r >> newColor.Ka_g >> newColor.Ka_b;
			}
			else if (sinput.compare("Kd") == 0)
			{
				mtlin >> newColor.Kd_r >> newColor.Kd_g >> newColor.Kd_b;
			}
			else if (sinput.compare("Ks") == 0)
			{
				mtlin >> newColor.Ks_r >> newColor.Ks_g >> newColor.Ks_b;
			}
			else if (sinput.compare("map_Kd") == 0)
			{
				mtlin >> newColor.map_Kd;
			}

			mtlin >> sinput;
		}

		if (newColor.map_Kd.compare("") == 0)
		{
			newColor.map_Kd = "material";
		}

		(*m_allMaterials)[materialName] = newColor;
	}

	// Close the file.
	mtlin.close();

	return true;
}

bool Exporter::LoadObjFileData(string filename)
{
	string filepath, sinput;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex;
	bool result;
	char input;

	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	filepath = INPUT_FOLDER + "\\" + filename;

	// Open the file.
	fin.open(filepath);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		cerr << "Could not find file: " + filepath << endl;
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin >> sinput;
	while (!fin.eof())
	{
		// Read in the vertices.
		if (sinput.compare("v") == 0)
		{
			VertexType vertex;
			double verx, very, verz;
			fin >> verx >> very >> verz;

			vertex.x = (float) verx;
			vertex.y = (float) very;
			vertex.z = (float) verz;

			// Invert the Z vertex to change to left hand system.
			vertex.z = vertex.z * -1.0f;
			m_vertices->push_back(vertex);
			vertexIndex++;
		}

		// Read in the texture uv coordinates.
		else if (sinput.compare("vt") == 0)
		{
			VertexType tex;
			double texx, texy;
			fin >> texx >> texy;

			tex.x = (float)texx;
			tex.y = (float)texy;

			// Invert the V texture coordinates to left hand system.
			tex.y = 1.0f - tex.y;
			m_texcoords->push_back(tex);
			texcoordIndex++;
		}

		// Read in the normals.
		else if (sinput.compare("vn") == 0)
		{
			VertexType normal;
			double norx, nory, norz;
			fin >> norx >> nory >> norz;

			normal.x = (float)norx;
			normal.y = (float)nory;
			normal.z = (float)norz;

			// Invert the Z normal to change to left hand system.
			normal.z = normal.z * -1.0f;
			m_normals->push_back(normal);
			normalIndex++;
		}

		// Read in the faces.
		else if (sinput.compare("f") == 0)
		{
			FaceType face;
			// Read the face data in backwards to convert it to a left hand system from right hand system.
			fin >> face.vIndex3 >> input >> face.tIndex3 >> input >> face.nIndex3
				>> face.vIndex2 >> input >> face.tIndex2 >> input >> face.nIndex2
				>> face.vIndex1 >> input >> face.tIndex1 >> input >> face.nIndex1;

			m_faces->push_back(face);
			faceIndex++;
		}

		else if (sinput.compare("usemtl") == 0)
		{
			fin >> sinput;
			(*m_materialFaceIndex)[faceIndex] = sinput;
		}

		else if (sinput.compare("mtllib") == 0)
		{
			fin >> sinput;
			string fullFilename = sinput;

			while ((fin.peek() != '\n'))
			{
				fin >> sinput;
				fullFilename = fullFilename + " " + sinput;
			}
			string mtlfilepath = INPUT_FOLDER + "\\" + fullFilename;

			result = LoadMaterialsFromFile(mtlfilepath);
			if (!result)
				return false;
		}

		else if (sinput.compare("object") == 0)
		{
			fin >> sinput;
			string objectName = sinput;

			while ((fin.peek() != '\n'))
			{
				fin >> sinput;
				objectName = objectName + " " + sinput;
			}
			(*m_objectIndex)[faceIndex] = objectName;

			//Skip the #
			fin >> sinput;
		}

		else if (sinput.compare("o") == 0 || sinput.compare("g") == 0)
		{
			fin >> sinput;
			string objectName = sinput;

			while ((fin.peek() != '\n'))
			{
				fin >> sinput;
				objectName = objectName + " " + sinput;
			}

			(*m_objectIndex)[faceIndex] = objectName;
		}

		// Start reading the beginning of the next line.
		fin >> sinput;
	}

	// Close the file.
	fin.close();

	return true;
}

bool Exporter::WriteOutputFile(string filepath)
{
	ofstream fout;
	int vIndex, tIndex, nIndex;

	if (FileExists(filepath))
	{
		if (remove(filepath.c_str()) != 0)
		{
			cerr << "Error removing " << filepath << endl;
			return false;
		}
	}

	// Open the output file.
	fout.open(filepath);

	// Check if it was successful in opening the file.
	if (fout.fail() == true)
	{
		cerr << "Could not create file: " + filepath << endl;
		return false;
	}

	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (m_faces->size() * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	vector<FaceType>::iterator it;

	// Now loop through all the faces and output the three vertices for each face.
	for (std::vector<FaceType>::size_type i = 0; i != m_faces->size(); i++)
	{
		if (m_objectIndex->find(i) != m_objectIndex->end())
		{
			fout << "o " << (*m_objectIndex)[i] << endl;
		}

		if (m_materialFaceIndex->find(i) != m_materialFaceIndex->end())
		{
			fout << "mtl " << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ns << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ka_r << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ka_g <<
				' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ka_b << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Kd_r << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Kd_g << ' ' <<
				(*m_allMaterials)[(*m_materialFaceIndex)[i]].Kd_b << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ks_r << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ks_g <<
				' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ks_b << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ni << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].d << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].illum << endl;
		}

		vIndex = (*m_faces)[i].vIndex1 - 1;
		tIndex = (*m_faces)[i].tIndex1 - 1;
		nIndex = (*m_faces)[i].nIndex1 - 1;

		fout << "vtn " << (*m_vertices)[vIndex].x << ' ' << (*m_vertices)[vIndex].y << ' ' << (*m_vertices)[vIndex].z << ' '
			<< (*m_texcoords)[tIndex].x << ' ' << (*m_texcoords)[tIndex].y << ' '
			<< (*m_normals)[nIndex].x << ' ' << (*m_normals)[nIndex].y << ' ' << (*m_normals)[nIndex].z << endl;

		vIndex = (*m_faces)[i].vIndex2 - 1;
		tIndex = (*m_faces)[i].tIndex2 - 1;
		nIndex = (*m_faces)[i].nIndex2 - 1;

		fout << "vtn " << (*m_vertices)[vIndex].x << ' ' << (*m_vertices)[vIndex].y << ' ' << (*m_vertices)[vIndex].z << ' '
			<< (*m_texcoords)[tIndex].x << ' ' << (*m_texcoords)[tIndex].y << ' '
			<< (*m_normals)[nIndex].x << ' ' << (*m_normals)[nIndex].y << ' ' << (*m_normals)[nIndex].z << endl;

		vIndex = (*m_faces)[i].vIndex3 - 1;
		tIndex = (*m_faces)[i].tIndex3 - 1;
		nIndex = (*m_faces)[i].nIndex3 - 1;

		fout << "vtn " << (*m_vertices)[vIndex].x << ' ' << (*m_vertices)[vIndex].y << ' ' << (*m_vertices)[vIndex].z << ' '
			<< (*m_texcoords)[tIndex].x << ' ' << (*m_texcoords)[tIndex].y << ' '
			<< (*m_normals)[nIndex].x << ' ' << (*m_normals)[nIndex].y << ' ' << (*m_normals)[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();

	return true;
}