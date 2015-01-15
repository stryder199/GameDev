#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <assert.h>
#include "..\..\WindowsHelpers\WindowsHelpers.h"

using namespace std;

static string GAMEDEV_FOLDER = "D:\\Users\\Mark\\Desktop\\GameDevHell";
static string SKYDRIVE_FOLDER = "D:\\Users\\Mark\\SkyDrive";
static string INPUT_FOLDER = SKYDRIVE_FOLDER + "\\3D_Models\\OBJ_MTL";
static string OUTPUT_FOLDER = GAMEDEV_FOLDER + "\\Formatted_3D_Models";
static string GAME_DATA_FOLDER = GAMEDEV_FOLDER + "\\NewGame\\Engine\\data";

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
	void LoadDataStructures(string);
    void LoadMaterialsFromFile(string mtlFilepath);
    void LoadObjFileData(string filename);
    void WriteOutputFile(string filepath);

private:
	map<string, ColorType> *m_allMaterials;
	map<int, string> *m_materialFaceIndex;
	map<int, string> *m_objectIndex;
	vector<VertexType> *m_vertices, *m_texcoords, *m_normals, *m_guns;
	vector<FaceType> *m_faces;
};