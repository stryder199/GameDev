#pragma once

#include <map>
#include <vector>

static std::string GAMEDEV_FOLDER = "D:\\Users\\Mark\\Desktop\\GameDevHell";
static std::string SKYDRIVE_FOLDER = "D:\\Users\\Mark\\SkyDrive";
static std::string INPUT_FOLDER = SKYDRIVE_FOLDER + "\\3D_Models\\OBJ_MTL";
static std::string OUTPUT_FOLDER = GAMEDEV_FOLDER + "\\Formatted_3D_Models";
static std::string GAME_DATA_FOLDER = GAMEDEV_FOLDER + "\\Engine\\data";

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
        std::string std::map_Kd;
    }ColorType;

public:
    Exporter();
    ~Exporter();
    void LoadDataStructures(std::string);
    void LoadMaterialsFromFile(std::string mtlFilepath);
    void LoadObjFileData(std::string filename);
    void WriteOutputFile(std::string filepath);

private:
    std::map<std::string, ColorType> *m_allMaterials;
    std::map<int, std::string> *m_materialFaceIndex;
    std::map<int, std::string> *m_objectIndex;
    std::vector<VertexType> *m_vertices, *m_texcoords, *m_normals, *m_guns;
    std::vector<FaceType> *m_faces;
};