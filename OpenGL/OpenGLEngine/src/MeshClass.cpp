#include "MeshClass.h"
#include "VectorClass.h"

MeshClass::MeshClass()
{
	mVertices = 0;
	mTexturesCord = 0;
	mNormals = 0;
	mTangents = 0;
	mBinomials = 0;
}

MeshClass::~MeshClass(void)
{
}

bool MeshClass::Initialize(string filename, Vector3 *vecColor)
{
	bool result;

	VertexType color = {vecColor->x, vecColor->y, vecColor->z};

	mVertices = new vector<VertexType>();
	if(!mVertices)
		return false;

	mTexturesCord = new vector<TextureData>();
	if(!mTexturesCord)
		return false;

	mNormals = new vector<VertexType>();
	if(!mNormals)
		return false;

	mIndices = new vector<unsigned int>();
	if(!mNormals)
		return false;

	mColors = new vector<VertexType>();
	if(!mColors)
		return false;

	mTangents = new vector<VertexType>();
	if(!mTangents)
		return false;

	mBinomials = new vector<VertexType>();
	if(!mBinomials)
		return false;

	result = loadMesh(filename, color);
	if(!result)
		return false;

	return true;
}

void MeshClass::Shutdown()
{
}

bool MeshClass::loadMesh(string filename, VertexType color)
{
	bool result;

	result = LoadDataStructures(filename, color);
	if(!result)
	{
		cerr << "Unable to load Mesh Data Structures" << endl;
		return false;
	}

	return true;
}

int MeshClass::getVertexCount()
{
	return mVertexCount;
}

bool MeshClass::LoadDataStructures(string filename, VertexType color)
{
	ifstream fin;
	int vIndex, tIndex, nIndex;
	char input, input2;

	vector<VertexType> vertices = vector<VertexType>();
	vector<VertexType> textures = vector<VertexType>();
	vector<VertexType> normals = vector<VertexType>();
	vector<FaceType> faces = vector<FaceType>();

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while(!fin.eof())
	{
		if(input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if(input == ' ')
			{
				VertexType vertex;
				fin >> vertex.x >> vertex.y >> vertex.z;

				// Invert the Z vertex to change to left hand system.
				vertex.z = vertex.z * -1.0f;
				vertices.push_back(vertex);
			}

			// Read in the texture uv coordinates.
			if(input == 't')
			{
				VertexType texture;
				fin >> texture.x >> texture.y;

				// Invert the V texture coordinates to left hand system.
				texture.y = 1.0f - texture.y;
				textures.push_back(texture);
			}

			// Read in the normals.
			if(input == 'n')
			{
				VertexType normal;
				fin >> normal.x >> normal.y >> normal.z;

				// Invert the Z normal to change to left hand system.
				normal.z = normal.z * -1.0f;
				normals.push_back(normal);
			}
		}

		// Read in the faces.
		if(input == 'f')
		{
			fin.get(input);
			if(input == ' ')
			{
				FaceType Face1, Face2, Face3;
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> Face1.v >> input2 >> Face1.t >> input2 >> Face1.n;
				fin	>> Face2.v >> input2 >> Face2.t >> input2 >> Face2.n;
				fin	>> Face3.v >> input2 >> Face3.t >> input2 >> Face3.n;

				faces.push_back(Face3);
				faces.push_back(Face2);
				faces.push_back(Face1);
			}
		}

		// Read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Now loop through all the faces and output the three vertices for each face.
	for(int i=0; i<faces.size(); i++)
	{
		VertexType vertex, normal;
		TextureData texture;
		vIndex = faces[i].v - 1;
		tIndex = faces[i].t - 1;
		nIndex = faces[i].n - 1;

		vertex.x = vertices[vIndex].x;
		vertex.y =  vertices[vIndex].y;
		vertex.z = vertices[vIndex].z;
		texture.tu = textures[tIndex].x;
		texture.tv = textures[tIndex].y;
		normal.x = normals[nIndex].x;
		normal.y = normals[nIndex].y;
		normal.z = normals[nIndex].z;

		mVertices->push_back(vertex);
		mTexturesCord->push_back(texture);
		mIndices->push_back((unsigned int)i);
		mColors->push_back(color);
		mNormals->push_back(normal);

		/*if((i+1)%3 == 0)
		{
			Vector3 tangent = Vector3();
			Vector3 binormal = Vector3();
			Vector3 trueNormal = Vector3();
			CalculateTangentsBinomials(i-2, i-1, i, tangent, binormal);
			CalculateNormal(tangent, binormal, trueNormal);

			VertexType vTan, vBi, vNorm;

			vTan.x = tangent.x;
			vTan.y = tangent.y;
			vTan.z = tangent.z;
			vBi.x = binormal.x;
			vBi.y = binormal.y;
			vBi.z = binormal.z;
			vNorm.x = trueNormal.x;
			vNorm.y = trueNormal.y;
			vNorm.z = trueNormal.z;

			mTangents->push_back(vTan);
			mTangents->push_back(vTan);
			mTangents->push_back(vTan);

			mBinomials->push_back(vBi);
			mBinomials->push_back(vBi);
			mBinomials->push_back(vBi);

			mNormals->push_back(vNorm);
			mNormals->push_back(vNorm);
			mNormals->push_back(vNorm);
		}*/
	}

	mVertexCount = faces.size();

	return true;
}

void MeshClass::CalculateTangentsBinomials(int vIndex1, int vIndex2, int vIndex3, Vector3& tangent, Vector3& binormal)
{
	Vector3 v1, v2;
	float tuVector[2], tvVector[2];
	float den;
	float length;

	// Calculate the two vectors for this face.
	v1.x = (*mVertices)[vIndex2].x - (*mVertices)[vIndex1].x;
	v1.y = (*mVertices)[vIndex2].y - (*mVertices)[vIndex1].y;
	v1.z = (*mVertices)[vIndex2].z - (*mVertices)[vIndex1].z;

	v2.x = (*mVertices)[vIndex3].x - (*mVertices)[vIndex1].x;
	v2.y = (*mVertices)[vIndex3].y - (*mVertices)[vIndex1].y;
	v2.z = (*mVertices)[vIndex3].z - (*mVertices)[vIndex1].z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = (*mTexturesCord)[vIndex2].tu - (*mTexturesCord)[vIndex1].tu;
	tvVector[0] = (*mTexturesCord)[vIndex2].tv - (*mTexturesCord)[vIndex1].tv;

	tuVector[1] = (*mTexturesCord)[vIndex3].tu - (*mTexturesCord)[vIndex1].tu;
	tvVector[1] = (*mTexturesCord)[vIndex3].tv - (*mTexturesCord)[vIndex1].tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * v1.x - tvVector[0] * v2.x) * den;
	tangent.y = (tvVector[1] * v1.y - tvVector[0] * v2.y) * den;
	tangent.z = (tvVector[1] * v1.z - tvVector[0] * v2.z) * den;

	binormal.x = (tuVector[0] * v2.x - tuVector[1] * v1.x) * den;
	binormal.y = (tuVector[0] * v2.y - tuVector[1] * v1.y) * den;
	binormal.z = (tuVector[0] * v2.z - tuVector[1] * v1.z) * den;

	tangent.normalize();
	binormal.normalize();
}

void MeshClass::CalculateNormal(Vector3 tangent, Vector3 binormal, Vector3& normal)
{
	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal = cross(tangent, binormal);

	// Calculate the length of the normal.
	normal.normalize();
}


MeshClass::VertexType* MeshClass::getVertices()
{
	vector <VertexType>::iterator it = mVertices->begin();
	return &(*it);
}

MeshClass::TextureData* MeshClass::getTextures()
{
	vector <TextureData>::iterator it = mTexturesCord->begin();
	return &(*it);
}

MeshClass::VertexType* MeshClass::getNormals()
{
	vector <VertexType>::iterator it = mNormals->begin();
	return &(*it);
}

unsigned int* MeshClass::getIndices()
{
	vector <unsigned int>::iterator it = mIndices->begin();
	return &(*it);
}

MeshClass::VertexType* MeshClass::getColors()
{
	vector <VertexType>::iterator it = mColors->begin();
	return &(*it);
}

MeshClass::VertexType* MeshClass::getBinomials()
{
	vector <VertexType>::iterator it = mBinomials->begin();
	return &(*it);
}

MeshClass::VertexType* MeshClass::getTangents()
{
	vector <VertexType>::iterator it = mTangents->begin();
	return &(*it);
}

void MeshClass::getFace(int index, Vector4 *v1, Vector4 *v2, Vector4 *v3, Vector3 *normal)
{
	int v1Index = index*3;
	int v2Index = index*3 + 1;
	int v3Index = index*3 + 2;

	v1->x = (*mVertices)[v1Index].x;
	v1->y = (*mVertices)[v1Index].y;
	v1->z = (*mVertices)[v1Index].z;
	v1->w = 1.0f;

	v2->x = (*mVertices)[v2Index].x;
	v2->y = (*mVertices)[v2Index].y;
	v2->z = (*mVertices)[v2Index].z;
	v2->w = 1.0f;

	v3->x = (*mVertices)[v3Index].x;
	v3->y = (*mVertices)[v3Index].y;
	v3->z = (*mVertices)[v3Index].z;
	v3->w = 1.0f;

	normal->x = (*mNormals)[v1Index].x;
	normal->y = (*mNormals)[v1Index].y;
	normal->z = (*mNormals)[v1Index].z;
}

void MeshClass::resetColor(Vector3 newColor)
{
	vector<VertexType>::iterator it;

	for(it = mColors->begin(); it != mColors->end(); it++)
	{
		(*it).x = newColor.x;
		(*it).y = newColor.y;
		(*it).z = newColor.z;
	}
}