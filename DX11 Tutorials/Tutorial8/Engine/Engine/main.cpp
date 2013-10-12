////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////


//////////////
// INCLUDES //
//////////////
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace std;


string inputFolder = "D:\\Users\\Mark\\Desktop\\GameDevHell\\3D_Models";
string outputFolder = "D:\\Users\\Mark\\Desktop\\GameDevHell\\Formatted_3D_Models";

//////////////
// TYPEDEFS //
//////////////
typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

typedef struct
{
	float Ns;
	float Ka_r, Ka_g, Ka_b;
	float Kd_r, Kd_g, Kd_b;
	float Ks_r, Ks_g, Ks_b;
	float Ni, d, illum;
}ColorType;


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
bool ReadFileCounts(string, int&, int&, int&, int&);
bool LoadDataStructures(string, int, int, int, int);

vector<string> listFile(string dirPath, string extension);

//////////////////
// MAIN PROGRAM //
//////////////////
int main()
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;

	vector<string> filenameList = listFile(inputFolder, "obj");

	vector<string>::iterator it;

	for (it = filenameList.begin(); it != filenameList.end(); ++it)
	{

		// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
		result = ReadFileCounts((*it), vertexCount, textureCount, normalCount, faceCount);
		if(!result)
		{
			return -1;
		}

		// Now read the data from the file into the data structures and then output it in our model format.
		result = LoadDataStructures((*it), vertexCount, textureCount, normalCount, faceCount);
		if(!result)
		{
			return -1;
		}
	}

	

	// Notify the user the model has been converted.
	cout << "\nDo you wish to exit (y/n)? ";
	cin >> garbage;

	return 0;
}

vector<string> listFile(string dirPath, string extension)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	vector<string> fileList;

	string pathWildcard = dirPath + "\\*." + extension; 
	std::wstring stemp = std::wstring(pathWildcard.begin(), pathWildcard.end());
	LPCWSTR lpathWildcard = stemp.c_str();

	hFind = FindFirstFile(lpathWildcard, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			WCHAR* wc = data.cFileName;

			//convert from wide char to narrow char array
			char ch[260];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, NULL);

			//A std:string  using the char* constructor.
			std::string ss(ch);

			fileList.push_back(ss);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	return fileList;
}

bool ReadFileCounts(string filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	string filePath = inputFolder + "\\" + filename;

	// Open the file.
	fin.open(filePath);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while(!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if(input == 'v')
		{
			fin.get(input);
			if(input == ' ') { vertexCount++; }
			if(input == 't') { textureCount++; }
			if(input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if(input == 'f')
		{
			fin.get(input);
			if(input == ' ') { faceCount++; }
		}
		
		// Otherwise read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}


bool LoadDataStructures(string filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin, mtlin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;
	string mtlfilename, outputfilename, mtlfilepath, filepath, outputfilepath;
	string sinput = "";
	bool mtlExisits = false;

	map<string, ColorType> allMaterials;
	map<int, string> materialFaceIndex;

	mtlfilename = filename;

	mtlfilename.erase(mtlfilename.end() - 4, mtlfilename.end());

	mtlfilename += ".mtl";

	mtlfilepath = inputFolder + "\\" + mtlfilename;

	// Read in the materials in the mtl file
	mtlin.open(mtlfilepath);

	// Check if it was successful in opening the file.
	if (mtlin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.

	while (!mtlin.eof() && !mtlExisits)
	{
		mtlin >> sinput;
		// Look for the start of a new material
		if (sinput.compare("newmtl") == 0)
		{
			mtlExisits = true;
		}	
	}

	while (!mtlin.eof() && mtlExisits == true)
	{
		ColorType newColor;
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

			mtlin >> sinput;
		}

		allMaterials[materialName] = newColor;
	}

	// Close the file.
	mtlin.close();

	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if(!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if(!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if(!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if(!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	filepath = inputFolder + "\\" + filename;

	// Open the file.
	fin.open(filepath);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin >> sinput;
	while(!fin.eof())
	{
		// Read in the vertices.
		if (sinput.compare("v") == 0)
		{ 
			fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

			// Invert the Z vertex to change to left hand system.
			vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
			vertexIndex++; 
		}

		// Read in the texture uv coordinates.
		if (sinput.compare("vt") == 0)
		{ 
			fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

			// Invert the V texture coordinates to left hand system.
			texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
			texcoordIndex++; 
		}

		// Read in the normals.
		if (sinput.compare("vn") == 0)
		{ 
			fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

			// Invert the Z normal to change to left hand system.
			normals[normalIndex].z = normals[normalIndex].z * -1.0f;
			normalIndex++; 
		}

		// Read in the faces.
		if (sinput.compare("f") == 0)
		{

			// Read the face data in backwards to convert it to a left hand system from right hand system.
			fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
				>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
				>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
			faceIndex++;
		}

		if (sinput.compare("usemtl") == 0)
		{
			fin >> sinput;
			materialFaceIndex[faceIndex] = sinput;
		}

		// Start reading the beginning of the next line.
		fin >> sinput;
	}

	// Close the file.
	fin.close();

	outputfilename = filename;

	outputfilename.erase(outputfilename.end() - 4, outputfilename.end());

	outputfilename += ".3dmodel";

	outputfilepath = outputFolder + "\\" + outputfilename;

	// Open the output file.
	fout.open(outputfilepath);
	
	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for(int i=0; i<faceIndex; i++)
	{
		if (materialFaceIndex.find(i) != materialFaceIndex.end())
		{
			fout << "mtl " << allMaterials[materialFaceIndex[i]].Ns << ' ' << allMaterials[materialFaceIndex[i]].Ns << ' ' << allMaterials[materialFaceIndex[i]].Ka_r << ' ' <<
				allMaterials[materialFaceIndex[i]].Ka_g << ' ' << allMaterials[materialFaceIndex[i]].Ka_b << ' ' << allMaterials[materialFaceIndex[i]].Kd_r << ' ' << 
				allMaterials[materialFaceIndex[i]].Kd_g << ' ' << allMaterials[materialFaceIndex[i]].Kd_b << ' ' << allMaterials[materialFaceIndex[i]].Ks_r << ' ' << allMaterials[materialFaceIndex[i]].Ks_g << 
				' ' << allMaterials[materialFaceIndex[i]].Ks_b << ' ' << allMaterials[materialFaceIndex[i]].Ni << ' ' << allMaterials[materialFaceIndex[i]].d << ' ' << allMaterials[materialFaceIndex[i]].illum << endl;
		}

		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << "vtn " << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			 << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			 << normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << "vtn " << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			 << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			 << normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << "vtn " << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			 << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			 << normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();

	// Release the four data structures.
	if(vertices)
	{
		delete [] vertices;
		vertices = 0;
	}
	if(texcoords)
	{
		delete [] texcoords;
		texcoords = 0;
	}
	if(normals)
	{
		delete [] normals;
		normals = 0;
	}
	if(faces)
	{
		delete [] faces;
		faces = 0;
	}

	return true;
}