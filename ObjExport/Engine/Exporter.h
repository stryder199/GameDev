#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

const string INPUT_FOLDER = "D:\\Users\\Mark\\Desktop\\GameDevHell\\3D_Models\\OBJ_MTL";
const string OUTPUT_FOLDER = "D:\\Users\\Mark\\Desktop\\GameDevHell\\Formatted_3D_Models";

class Exporter
{
	//////////////
	// TYPEDEFS //
	//////////////
public:
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
		string map_Kd;
	}ColorType;

public:
	Exporter();
	~Exporter();
	bool LoadDataStructures(string);
	bool LoadMaterialsFromFile(string mtlFilepath);
	bool LoadObjFileData(string filename);
	bool WriteOutputFile(string filepath);

private:
	map<string, ColorType> *m_allMaterials;
	map<int, string> *m_materialFaceIndex;
	map<int, string> *m_objectIndex;
	vector<VertexType> *m_vertices, *m_texcoords, *m_normals;
	vector<FaceType> *m_faces;
};

static TCHAR* toTStr(string s)
{
	TCHAR *param = new TCHAR[s.size() + 1];
	param[s.size()] = 0;
	copy(s.begin(), s.end(), param);
	return param;
}

static bool FileExists(const string fileName)
{
	DWORD       fileAttr;

	fileAttr = GetFileAttributes(toTStr(fileName));
	if (0xFFFFFFFF == fileAttr)
		return false;
	return true;
}