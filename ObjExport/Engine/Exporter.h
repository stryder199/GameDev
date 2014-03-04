#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <assert.h>
#include "WindowsHelpers.h"

using namespace std;

static string INPUT_FOLDER = skydriveDir() + "\\3D_Models\\OBJ_MTL";
static string OUTPUT_FOLDER = "E:\\Personal\\GameDev\\Formatted_3D_Models";
static string GAME_DATA_FOLDER = "E:\\Personal\\GameDev\\NewGame\\Engine\\data";
//static string OUTPUT_FOLDER = desktopDir() + "\\GameDevHell\\Formatted_3D_Models";
//static string GAME_DATA_FOLDER = desktopDir() + "\\GameDevHell\\NewGame\\Engine\\data";

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
	vector<VertexType> *m_vertices, *m_texcoords, *m_normals, *m_guns;
	vector<FaceType> *m_faces;
};