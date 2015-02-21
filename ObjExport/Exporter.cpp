#include "Exporter.h"
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
#include "..\Shared\WindowsHelpers.h"
#include "boost\algorithm\string.hpp"
#include "..\Shared\GenericException.h"

using namespace std;
using namespace boost;

Exporter::Exporter()
{
    m_allMaterials = new map<string, ColorType>();
    m_materialFaceIndex = new map<int, string>();
    m_objectIndex = new map<int, string>();
    m_vertices = new vector<VertexType>();
    m_texcoords = new vector<VertexType>();
    m_normals = new vector<VertexType>();
    m_faces = new vector<FaceType>();
    m_guns = new vector<VertexType>();
}

Exporter::~Exporter()
{
}

void Exporter::LoadDataStructures(string filename)
{
    string outputfilename, outputfilepath;

    LoadObjFileData(filename);

    outputfilename = filename;

    outputfilename.erase(outputfilename.end() - 4, outputfilename.end());

    outputfilename += ".3dmodel";

    outputfilepath = OUTPUT_FOLDER + "\\" + outputfilename;

    WriteOutputFile(outputfilepath);
}

void Exporter::LoadMaterialsFromFile(string mtlFilepath)
{
    string sinput = "";
    ifstream mtlin;

    // Read in the materials in the mtl file
    mtlin.open(mtlFilepath);
    if (mtlin.fail())
    {
        throw GenericException("File not found");
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
                mtlin >> sinput;
                string fullFilename = sinput;

                while (mtlin.peek() != '\n')
                {
                    mtlin >> sinput;
                    fullFilename = fullFilename + " " + sinput;
                }

                if (fullFilename.find(":\\") != string::npos)
                {
                    vector<string> SplitVec;
                    split(SplitVec, fullFilename, is_any_of("\\"), token_compress_on);
                    fullFilename = SplitVec[SplitVec.size() - 2] + "\\" + SplitVec[SplitVec.size() - 1];
                }

                newColor.map_Kd = fullFilename;
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
}

void Exporter::LoadObjFileData(string filename)
{
    string filepath, sinput;
    ifstream fin;
    int vertexIndex, texcoordIndex, normalIndex, faceIndex;
    char input;

    vertexIndex = 0;
    texcoordIndex = 0;
    normalIndex = 0;
    faceIndex = 0;

    filepath = INPUT_FOLDER + "\\" + filename;

    // Open the file.
    fin.open(filepath);
    if (fin.fail())
    {
        throw GenericException("File not found");
    }

    // Read in the vertices, texture coordinates, and normal's into the data structures.
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

        // Read in the normal's.
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

            LoadMaterialsFromFile(mtlfilepath);
        }
        else if (sinput.compare("o") == 0 || sinput.compare("g") == 0 || sinput.compare("object") == 0)
        {
            fin >> sinput;
            string objectName = sinput;

            while ((fin.peek() != '\n'))
            {
                fin >> sinput;
                objectName = objectName + " " + sinput;
            }

            if (objectName.find("Gun") != string::npos)
            {
                VertexType gunPosition;
                fin >> sinput;
                assert(sinput.compare("v") == 0);
                
                double verx, very, verz;
                fin >> verx >> very >> verz;

                gunPosition.x = (float) verx;
                gunPosition.y = (float) very;
                gunPosition.z = (float) verz;

                // Invert the Z vertex to change to left hand system.
                gunPosition.z = gunPosition.z * -1.0f;

                m_guns->push_back(gunPosition);

                // Make sure that the face index is correct
                m_vertices->push_back(gunPosition);
                vertexIndex++;
            }
            else
            {
                (*m_objectIndex)[faceIndex] = objectName;

                if (sinput.compare("object") == 0)
                {
                    //Skip the #
                    fin >> sinput;
                }
            }
        }

        // Start reading the beginning of the next line.
        fin >> sinput;
    }

    // Close the file.
    fin.close();
}

void Exporter::WriteOutputFile(string filepath)
{
    ofstream fout;
    int vIndex, tIndex, nIndex;

    if (WindowsHelpers::FileExists(filepath))
    {
        remove(filepath.c_str());
    }

    // Open the output file.
    fout.open(filepath);
    if (fout.fail())
    {
        throw GenericException("Failed to open file");
    }

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
                ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ks_b << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].Ni << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].d <<
                ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].illum << ' ' << (*m_allMaterials)[(*m_materialFaceIndex)[i]].map_Kd << endl;
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

    vector<VertexType>::iterator gun;
    for (gun = m_guns->begin(); gun != m_guns->end(); ++gun)
    {
        fout << "gun " << (*gun).x << ' ' << (*gun).y << ' ' << (*gun).z << endl;
    }

    fout << "end" << endl;

    // Close the output file.
    fout.close();
}