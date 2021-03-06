#include "MeshClass.h"
#include "MaterialClass.h"
#include "TextureClass.h"
#include "ObjectMeshClass.h"
#include "MeshDataClass.h"
#include "WindowsHelpers.h"
#include "D3DClass.h"
#include "GenericException.h"
#include <fstream>

using namespace std;
using namespace DirectX;

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

XMFLOAT3 readGunLine(string::iterator* it)
{
    //Load vertices until a new object starts
    XMFLOAT3 newVertex = XMFLOAT3();

    newVertex.x = readFloat(it);
    newVertex.y = readFloat(it);
    newVertex.z = readFloat(it);

    return newVertex;
}

void MeshClass::Initialize(string meshFilename, MeshClass::MeshType type)
{
    m_type = type;

    m_allObjects = vector<ObjectMeshClass*>();
    m_guns = vector<XMFLOAT3>();

    // Load in the model data,
    LoadModel(meshFilename);

    InitializeBuffers();
}

void MeshClass::Initialize(ObjectMeshClass *object, MeshClass::MeshType type)
{
    m_type = type;

    m_allObjects = vector<ObjectMeshClass*>();
    m_guns = vector<XMFLOAT3>();

    m_allObjects.push_back(object);

    InitializeBuffers();
}

void MeshClass::InitializeBuffers()
{
    // For each object in the mesh
    vector<ObjectMeshClass*>::iterator object;
    for (object = m_allObjects.begin(); object != m_allObjects.end(); ++object)
    {
        // For each submesh
        vector<MeshDataClass*>::iterator subMesh;
        vector<MeshDataClass*>* allSubmeshs = (*object)->getAllMeshData();
        for (subMesh = allSubmeshs->begin(); subMesh != allSubmeshs->end(); ++subMesh)
        {
            unsigned long* indices;
            VertexTextureType* texVertices;
            VertexMaterialType* matVertices;
            VertexTextType* textVertices;
            D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
            D3D11_SUBRESOURCE_DATA vertexData, indexData;
            ID3D11Buffer *vertexBuf, *indexBuf;
            HRESULT result;

            // Create the index array.
            indices = new unsigned long[(*subMesh)->getIndexCount()];
            if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
            {
                // Create the vertex array.
                matVertices = new VertexMaterialType[(*subMesh)->getVertexCount()];

                // For each mesh data
                vector<MeshDataClass::MeshDataType>::iterator rawMeshData;
                vector<MeshDataClass::MeshDataType>* allMeshData = (*subMesh)->getRawMeshData();

                int count = 0;
                // Load the vertex array and index array with data.
                for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
                {
                    matVertices[count].position = XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
                    matVertices[count].normals = XMFLOAT3((*rawMeshData).nx, (*rawMeshData).ny, (*rawMeshData).nz);

                    indices[count] = count;
                    count++;
                }

                // Set up the description of the static vertex buffer.
                vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                vertexBufferDesc.ByteWidth = sizeof(VertexMaterialType) * (*subMesh)->getVertexCount();
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                // Give the sub resource structure a pointer to the vertex data.
                vertexData.pSysMem = matVertices;
                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;
            }
            else if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
            {
                // Create the vertex array.
                texVertices = new VertexTextureType[(*subMesh)->getVertexCount()];
                // For each mesh data
                vector<MeshDataClass::MeshDataType>::iterator rawMeshData;
                vector<MeshDataClass::MeshDataType>* allMeshData = (*subMesh)->getRawMeshData();
                int count = 0;
                // Load the vertex array and index array with data.
                for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
                {
                    texVertices[count].position = XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
                    texVertices[count].texture = XMFLOAT2((*rawMeshData).tu, (*rawMeshData).tv);
                    texVertices[count].normals = XMFLOAT3((*rawMeshData).nx, (*rawMeshData).ny, (*rawMeshData).nz);

                    indices[count] = count;
                    count++;
                }

                // Set up the description of the static vertex buffer.
                vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * (*subMesh)->getVertexCount();
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                // Give the sub resource structure a pointer to the vertex data.
                vertexData.pSysMem = texVertices;
                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;
            }
            else if (m_type == MeshClass::TEXT || m_type == MeshClass::TWOD)
            {
                // Create the vertex array.
                textVertices = new VertexTextType[(*subMesh)->getVertexCount()];
                // For each mesh data
                vector<MeshDataClass::MeshDataType>::iterator rawMeshData;
                vector<MeshDataClass::MeshDataType>* allMeshData = (*subMesh)->getRawMeshData();
                int count = 0;
                // Load the vertex array and index array with data.
                for (rawMeshData = allMeshData->begin(); rawMeshData != allMeshData->end(); ++rawMeshData)
                {
                    textVertices[count].position = XMFLOAT3((*rawMeshData).x, (*rawMeshData).y, (*rawMeshData).z);
                    textVertices[count].texture = XMFLOAT2((*rawMeshData).tu, (*rawMeshData).tv);

                    indices[count] = count;
                    count++;
                }

                // Set up the description of the static vertex buffer.
                vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                vertexBufferDesc.ByteWidth = sizeof(VertexTextType) * (*subMesh)->getVertexCount();
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                // Give the sub resource structure a pointer to the vertex data.
                vertexData.pSysMem = textVertices;
                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;
            }

            // Now create the vertex buffer.
            result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuf);
            if (FAILED(result))
            {
                throw GenericException("Failed to create the vertex buffer.");
            }

            (*subMesh)->setVertexBuffer(vertexBuf);

            // Set up the description of the static index buffer.
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*subMesh)->getIndexCount();
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            // Give the sub resource structure a pointer to the index data.
            indexData.pSysMem = indices;
            indexData.SysMemPitch = 0;
            indexData.SysMemSlicePitch = 0;

            // Create the index buffer.
            result = D3DClass::getInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuf);
            if (FAILED(result))
            {
                throw GenericException("Failed to create the index buffer.");
            }

            (*subMesh)->setIndexBuffer(indexBuf);

            if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::MATERIAL)
            {
                delete[] matVertices;
                matVertices = 0;
            }
            else if (m_type == MeshClass::THREED && (*subMesh)->getMeshColorType() == MeshDataClass::TEXTURE)
            {
                delete[] texVertices;
                texVertices = 0;
            }
            else if (m_type == MeshClass::TEXT || m_type == MeshClass::TWOD)
            {
                delete[] textVertices;
                textVertices = 0;
            }

            delete[] indices;
            indices = 0;
        }
    }
}

void MeshClass::Shutdown()
{
    vector<ObjectMeshClass*>::iterator it;
    for (it = m_allObjects.begin(); it != m_allObjects.end(); ++it)
    {
        (*it)->Shutdown();
    }

    return;
}

void MeshClass::LoadModel(string filename)
{
    string sinput;
    string s;

    ifstream is(filename, ifstream::binary);
    if (is.fail())
    {
        throw GenericException("File not found");
    }

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
            focusObject->Initialize(objectName);

            //Should auto skip to the next line
            
            //Check if you need to use the prev material
            if ((*it) == 'v')
            {
                focusMesh = new MeshDataClass();

                if (focusType == MeshDataClass::MeshColorType::MATERIAL)
                {
                    focusMesh->Initialize(focusMaterial);
                }
                else
                {
                    focusMesh->Initialize(focusTexture);
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
                material->Initialize(focusColorInfo);

                focusType = MeshDataClass::MeshColorType::MATERIAL;

                focusMesh->Initialize(material);
            }
            else
            {
                TextureClass* texture = new TextureClass();
                texture->Initialize(WindowsHelpers::ToLWideStr("data\\" + focusColorInfo.map_Kd));

                focusTexture = texture;
                focusType = MeshDataClass::MeshColorType::TEXTURE;

                focusMesh->Initialize(texture);
            }

            //Should auto skip to the next line
        }    
        else if (sinput.compare("gun") == 0)
        {
            XMFLOAT3 newGun = readGunLine(&it);
            m_guns.push_back(newGun);
        }
    }

    //Add the last objects
    focusObject->addMesh(focusMesh);
    m_allObjects.push_back(focusObject);
}

vector<ObjectMeshClass*>* MeshClass::getAllObjects()
{
    return &m_allObjects;
}

vector<XMFLOAT3>* MeshClass::getGuns()
{
    return &m_guns;
}

MeshClass::MeshType MeshClass::getMeshType()
{
    return m_type;
}