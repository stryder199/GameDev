#include "ModelClass.h"
#include "MeshClass.h"
#include "MatrixClass.h"
#include "VectorClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"
#include "ShaderProgram.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "BumpMapClass.h"
#include "RayTracer.h"
#include "Ray.h"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <Windows.h>
#endif

ModelClass::ModelClass()
{
	mScale = 0;
	mTranslation = 0;
	mRotation = 0;
	mMesh = 0;
	mTexture = 0;
}

ModelClass::~ModelClass(void)
{
}

bool ModelClass::Initialize( string meshFilename, string textureFilename, string bumpmapFilename, LightClass* lightSource )
{
	bool result;

	mLightSource = lightSource;

	mScale = new Vector3(1.0f, 1.0f, 1.0f);
	mTranslation = new Vector3(0.0f, 0.0f, 0.0f);
	mRotation = new Vector3(0.0f, 0.0f, 0.0f);
	color = new Vector3(0.58039f, 0.0f, 0.82745f);

	mPrimitive = ModelClass::TRIANGLES;

	mPost = false;
	mOrtho = false;

	mPerspectiveMatrix = new Matrix4x4();
	mOrthoMatrix = new Matrix4x4();

	// Set the field of view and screen aspect ratio.
	float fieldOfView = 3.14159265358979323846f / 4.0f;
	float screenAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	mPerspectiveMatrix->setPerspective(fieldOfView, screenAspect, (float)SCREEN_NEAR, (float)SCREEN_DEPTH);
	mOrthoMatrix->setOrthoProj(screenAspect, (float)SCREEN_NEAR, (float)SCREEN_DEPTH);

	mMesh = new MeshClass();
	if(!mMesh)
	{
		cerr << "Mesh Obj Creation Failure" << endl;
		return false;
	}

	result = mMesh->Initialize(meshFilename, color);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Mesh Init Failure", "Error", MB_OK );
#endif
		cerr << "Mesh Init Failure" << endl;
		return false;
	}

	/*mTexture = new TextureClass();
	if(!mTexture)
	{
		return false;
	}

	result = mTexture->Initialize(textureFilename, bumpmapFilename, 0, true, 1, false);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Texture Init Failure", "Error", MB_OK );
#endif
		return false;
	}*/

	initOpenGLBuffers();

	mMeshFilename = meshFilename;
	mTexFilename = textureFilename;
	mName = parseFilename();

	if(mName == "sphere.obj")
	{
		objType = SPHERE;
		radius = 1.0;
	}
	else if(mName == "smallsquare.obj")
	{
		objType = FLOOR;
	}
	else{
		objType = OTHER;
	}


	return true;
}

void ModelClass::Shutdown()
{
}

bool  ModelClass::computeIntersect(Ray ray, CameraClass* camera, Vector3* hitPoint, Vector3* inputNormal, Vector3* viewDirection)
{
	int faceCount = mMesh->getVertexCount()/3;
	bool result;
	Ray worldRay;
	Matrix4x4 invWorld, positionMatrix, projMatrix;

	Matrix4x4 worldMatrix;
	worldMatrix.setIdentity();
	worldMatrix.scale(mScale->x, mScale->y, mScale->z);

	Matrix4x4 translation;
	translation.setIdentity();
	translation.translate(mTranslation->x, mTranslation->y, mTranslation->z);
	worldMatrix *= translation;

	Matrix4x4 rotation;
	rotation.setIdentity();
	rotation.rotateAllAxis(mRotation->x, mRotation->y, mRotation->z);
	worldMatrix *= rotation;

	invWorld = worldMatrix.getInverse();
	Matrix3x3 invMat3 = mat3(invWorld);
	worldRay = ray;
	worldRay.direction = worldRay.direction*invMat3;
	worldRay.origin = worldRay.origin*invMat3;


	// Set the field of view and screen aspect ratio.
	float fieldOfView = 3.14159265358979323846f / 4.0f;
	float screenAspect = (float)FRAME_WIDTH / (float)FRAME_HEIGHT;
	projMatrix.setPerspective(fieldOfView, screenAspect, (float)SCREEN_NEAR, (float)SCREEN_DEPTH);

	if(objType == SPHERE)
	{
		Vector3 intersectPoint1, intersectPoint2;
		result = computeSphereIntersect(worldRay, &intersectPoint1, &intersectPoint2, inputNormal);
		if(result == true)
		{
			*viewDirection = camera->getPosition() - intersectPoint1;
			viewDirection->normalize();
			*hitPoint = intersectPoint1;
			return true;
		}
	}
	else if(objType == FLOOR)
	{
		Vector3 planeNormal = Vector3(0.0f, 1.0f, -0.00001f);
		planeNormal.normalize();
		Vector3 planeOrigin = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 intersectPoint;
		result = computePlaneIntersect(ray, planeNormal, planeOrigin, &intersectPoint);
		if(result == true)
		{
			*inputNormal = planeNormal;
			*viewDirection = camera->getPosition() - intersectPoint;
			viewDirection->normalize();
			*hitPoint = intersectPoint;
			return true;
		}
	}
	else{
		for(int i=0; i < faceCount; i++)
		{
			Vector4 v1, v2, v3, v1Position, v2Position, v3Position;
			Vector3 normal;
			mMesh->getFace(i, &v1, &v2, &v3, &normal);

			v1Position = v1;
			v2Position = v2;
			v3Position = v3;

			v1Position *= worldMatrix;
			v2Position *= worldMatrix;
			v3Position *= worldMatrix;

			v1Position *= camera->GetViewMatrix();			
			v2Position *= camera->GetViewMatrix();			
			v3Position *= camera->GetViewMatrix();

			v1Position *= projMatrix;
			v2Position *= projMatrix;
			v3Position *= projMatrix;

			normal *= mat3(worldMatrix);
			normal.normalize();

			result = computeTriangleIntersect(worldRay, v1Position, v2Position, v3Position);
			if(result == true)
			{
				Vector4 worldPosition;
				*inputNormal = normal;
				inputNormal->normalize();

				worldPosition = v1 * worldMatrix;

				*viewDirection = camera->getPosition() - vec3(worldPosition);
				viewDirection->normalize();
				return true;
			}
		}
	}
	return false;
}

bool ModelClass::computeShadowIntersect(Ray ray, CameraClass* camera)
{
	int faceCount = mMesh->getVertexCount()/3;
	bool result;
	Ray worldRay;
	Matrix4x4 invWorld, positionMatrix, projMatrix;

	Matrix4x4 worldMatrix;
	worldMatrix.setIdentity();
	worldMatrix.scale(mScale->x, mScale->y, mScale->z);

	Matrix4x4 translation;
	translation.setIdentity();
	translation.translate(mTranslation->x, mTranslation->y, mTranslation->z);
	worldMatrix *= translation;

	Matrix4x4 rotation;
	rotation.setIdentity();
	rotation.rotateAllAxis(mRotation->x, mRotation->y, mRotation->z);
	worldMatrix *= rotation;

	invWorld = worldMatrix.getInverse();
	Matrix3x3 invMat3 = mat3(invWorld);
	worldRay = ray;
	worldRay.direction = worldRay.direction*invMat3;
	worldRay.origin = worldRay.origin*invMat3;

	

	if(objType == SPHERE)
	{
		Vector3 intersectPoint1, intersectPoint2, normal;
		result = computeSphereIntersect(worldRay, &intersectPoint1, &intersectPoint2, &normal);
		if(result == true)
		{
			return true;
		}
	}
	else if(objType == FLOOR)
	{
		Vector3 planeNormal = Vector3(0.0f, 1.0f, -0.00001f);
		planeNormal.normalize();
		Vector3 planeOrigin = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 intersectPoint;
		result = computePlaneIntersect(ray, planeNormal, planeOrigin, &intersectPoint);
		if(result == true)
		{
			return true;
		}
	}
	else{
		// Set the field of view and screen aspect ratio.
		float fieldOfView = 3.14159265358979323846f / 4.0f;
		float screenAspect = (float)FRAME_WIDTH / (float)FRAME_HEIGHT;
		projMatrix.setPerspective(fieldOfView, screenAspect, (float)SCREEN_NEAR, (float)SCREEN_DEPTH);
		for(int i=0; i < faceCount; i++)
		{
			Vector4 v1, v2, v3, v1Position, v2Position, v3Position;
			Vector3 normal;
			mMesh->getFace(i, &v1, &v2, &v3, &normal);

			v1Position = v1;
			v2Position = v2;
			v3Position = v3;

			v1Position *= worldMatrix;
			v2Position *= worldMatrix;
			v3Position *= worldMatrix;

			v1Position *= camera->GetViewMatrix();			
			v2Position *= camera->GetViewMatrix();			
			v3Position *= camera->GetViewMatrix();

			v1Position *= projMatrix;
			v2Position *= projMatrix;
			v3Position *= projMatrix;

			normal *= mat3(worldMatrix);
			normal.normalize();

			result = computeTriangleIntersect(worldRay, v1Position, v2Position, v3Position);
			if(result == true)
			{
				return true;
			}
		}
	}
	return false;
}

bool ModelClass::computeSphereIntersect(Ray ray, Vector3* intersectPoint1, Vector3* intersectPoint2, Vector3* normal)
{
	int intersectPoints = 0;
	Vector3 e = ray.direction;   // e=ray.dir
	e.normalize();                            // e=g/|g|
	Vector3 h = *mTranslation - ray.origin;  // h=r.o-c.M
	float lf = e.dot(h);                      // lf=e.h
	float s = (radius*radius)-h.dot(h)+(lf*lf);   // s=r^2-h^2+lf^2
	
	if (s < 0.0) 
		return false;                    // no intersection points ?
	s = sqrt(s);                              // s=sqrt(r^2-h^2+lf^2)
 
	if (lf < s)                               // S1 behind A ?
	{ 
		if (lf+s >= 0)                          // S2 before A ?}
		{ 
			s = -s;                               // swap S1 <-> S2}
			intersectPoints = 1;                           // one intersection point
		} 
	}
	else 
		intersectPoints = 2;                          // 2 intersection points
 
	*intersectPoint1 = e * (lf-s);
	*intersectPoint1 += ray.origin;
	*intersectPoint2 = e * (lf+s);
	*intersectPoint2 += ray.origin;

	*normal = *intersectPoint1 - *mTranslation;  //closest point of the intersect minus the center of the sphere
	normal->normalize();
 
	return true;
}

bool ModelClass::computeTriangleIntersect(Ray ray, Vector4 inputv1, Vector4 inputv2, Vector4 inputv3)
{
	Vector3 edge1, edge2, tvec, pvec, qvec;
	Vector3 v1 = vec3(inputv1);
	Vector3 v2 = vec3(inputv2);
	Vector3 v3 = vec3(inputv3);

	float det, invDet, u, t, v;
	edge1 = v2-v1;
	edge2 = v3-v1;

	pvec = cross(ray.direction, edge2);

	det = edge1.dot(pvec);

	if(det < EPSILON)
	{
		return false;
	}

	tvec = ray.origin - v1;

	u = tvec.dot(pvec);

	qvec = cross(tvec, edge1);
	
	v = ray.direction.dot(qvec);

	if(v < 0.0 || u+v > det)
	{
		return false;
	}

	return true;
}

bool ModelClass::computePlaneIntersect(Ray ray, Vector3 planeNormal, Vector3 planeOrigin, Vector3* intersectPoint)
{
	// Calc D for the plane (this is usually pre-calculated
	// and stored with the plane)

	float D = planeOrigin.dot(planeNormal);

	// Determine the distance from rOrigin to the point of
	// intersection on the plane

	float numer = planeNormal.dot(ray.origin) + D;
	float denom = planeNormal.dot(ray.direction);
	float time = -(numer / denom);

	//Check if time < 0
	if(time < EPSILON || _isnan(time) != 0)
	{
		return false;
	}

	// Calculate the collision point
	Vector3 collisionPoint;
	collisionPoint.x = ray.origin.x + ray.direction.x * time;
	collisionPoint.y = ray.origin.y + ray.direction.y * time;
	collisionPoint.z = ray.origin.z + ray.direction.z * time;
	*intersectPoint = collisionPoint;

	return true;
}

bool ModelClass::Render(ShaderProgram* shaderProgram, CameraClass* camera)
{
	bool result;
	Matrix4x4 worldMatrix, projectMatrix;

	if(mOrtho == false)
	{
		projectMatrix = *mPerspectiveMatrix;
	}
	else
	{
		projectMatrix = *mOrthoMatrix;
	}

	shaderProgram->activate();

	worldMatrix.setIdentity();
	worldMatrix.scale(mScale->x, mScale->y, mScale->z);

	if(mPost == false)
	{
		Matrix4x4 translation;
		translation.setIdentity();
		translation.translate(mTranslation->x, mTranslation->y, mTranslation->z);
		worldMatrix *= translation;

		Matrix4x4 rotation;
		rotation.setIdentity();
		rotation.rotateAllAxis(mRotation->x, mRotation->y, mRotation->z);
		worldMatrix *= rotation;
	}
	else
	{
		Matrix4x4 rotation;
		rotation.setIdentity();
		rotation.rotateAllAxis(mRotation->x, mRotation->y, mRotation->z);
		worldMatrix *= rotation;

		Matrix4x4 translation;
		translation.setIdentity();
		translation.translate(mTranslation->x, mTranslation->y, mTranslation->z);
		worldMatrix *= translation;
	}

	result = shaderProgram->SetShaderParameters(worldMatrix, camera->GetViewMatrix(), projectMatrix, *mLightSource->getDirection(), *mLightSource->getDiffuseColor(), *mLightSource->getAmbientLight(), camera->getPosition(), *mLightSource->getSpecularColor(), mLightSource->getSpecularPower());
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Failed to Set Shader Params", "Error", MB_OK );
#endif
		return false;
	}

	glBindVertexArray(mVertexArray);

	//Vertex Data
	glGenBuffers(1, &mVertexBuffer);
	//Load the Vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh->getVertexCount()*sizeof(MeshClass::VertexType), mMesh->getVertices(), GL_STATIC_DRAW);
	//Enable Vertex Attrib
	glEnableVertexAttribArray(0); //Vertex input, position 0
	//Specify the format of the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//Normal Data
	glGenBuffers(1, &mNormalBuffer);
	//Load the Normal Data
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh->getVertexCount()*sizeof(MeshClass::VertexType), mMesh->getNormals(), GL_STATIC_DRAW);
	//Enable Vertex Attrib
	glEnableVertexAttribArray(1); //Normal Coord input, position 1
	//Specify the format of the data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Color Data
	glGenBuffers(1, &mColorBuffer);
	//Load the Color Data
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMesh->getVertexCount()*sizeof(MeshClass::VertexType), mMesh->getColors(), GL_STATIC_DRAW);
	//Enable Vertex Attrib
	glEnableVertexAttribArray(2); //Color Coord input, position 2
	//Specify the format of the data
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Index Data
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->getVertexCount()*sizeof(unsigned int), mMesh->getIndices(), GL_STATIC_DRAW);

	//Render the Scene
	//glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE ) ;
	glDrawElements(GL_TRIANGLES, mMesh->getVertexCount(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Disable vertex
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &mVertexBuffer);
	mVertexBuffer = 0;

	//Disable Normal
	glDisableVertexAttribArray(1);
	glDeleteBuffers(1, &mNormalBuffer);
	mNormalBuffer = 0;

	//Disable Color
	glDisableVertexAttribArray(2);
	glDeleteBuffers(1, &mColorBuffer);
	mColorBuffer = 0;

	glBindVertexArray(0);

	shaderProgram->deactivate();

	return true;
}

void ModelClass::initOpenGLBuffers()
{
	glGenVertexArrays(1, & mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, & mColorBuffer);
	glGenBuffers(1, & mNormalBuffer);
	glGenBuffers(1, & mTexCoordBuffer);
	glGenBuffers(1, & mVertexBuffer);
	glGenBuffers(1, & mIndexBuffer);
	glGenBuffers(1, & mTangentBuffer);
	glGenBuffers(1, & mBinormalBuffer);

	glBindVertexArray(0);
}

void ModelClass::translate(Vector3 m_Translation)
{
	for(int i=0; i<3; i++)
	{
		if(m_Translation.elements[i] != 0.0f)
		{
			mTranslation->elements[i] = m_Translation.elements[i];
			//return;
		}
	}
}

void ModelClass::setScaleFactor(Vector3 scaleVec)
{
	for(int i=0; i<3; i++)
	{
		if(scaleVec.elements[i] != 0.0f)
		{
			mScale->elements[i] = scaleVec.elements[i];
		}
	}
}

void ModelClass::rotate(Vector3 rotation)
{
	for(int i=0; i<3; i++)
	{
		if(rotation.elements[i] != 0.0f)
		{
			mRotation->elements[i] = rotation.elements[i];
			return;
		}
	}
}

void ModelClass::setOrtho(bool val)
{
	mOrtho = val;
}

void ModelClass::setPost(bool val)
{
	mPost = val;
}

void ModelClass::setColor(Vector3 newColor)
{
	*color = newColor;
	mMesh->resetColor(newColor);
}

string ModelClass::parseFilename()
{
	return mMeshFilename.substr(mMeshFilename.find_last_of("/") + 1, mMeshFilename.length() - mMeshFilename.find_last_of("/"));
}

string ModelClass::getMeshFilename()
{
	return mMeshFilename;
}

string ModelClass::getName()
{
	return mName;
}

Vector3 ModelClass::getColor()
{
	return *color;
}