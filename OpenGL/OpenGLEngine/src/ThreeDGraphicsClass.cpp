#include "ThreeDGraphicsClass.h"
#include "ModelClass.h"
#include "ShaderProgram.h"
#include "MatrixClass.h"
#include "LightClass.h"
#include "CameraClass.h"
#include "VectorClass.h"
#include "Ray.h"
#ifdef _WIN32
#include <Windows.h>
#endif

ThreeDGraphicsClass::ThreeDGraphicsClass(void)
{
	mAllModels = 0;
	mShaderProgram = 0;
	mLightSource = 0;
}

ThreeDGraphicsClass::~ThreeDGraphicsClass(void)
{
}

bool ThreeDGraphicsClass::Initialize()
{
	bool result;

	mAllModels = new vector<ModelClass*>;
	if(!mAllModels)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Model Vector Obj Creation Failure", "Error", MB_OK );
#endif
		return false;
	}

	mShaderProgram = new ShaderProgram();
	if(!mShaderProgram)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Shader Program Obj Creation Failure", "Error", MB_OK );
#endif
		return false;
	}

	result = mShaderProgram->Initialize("./shaders/VertexShader.glsl", "./shaders/FragmentShader.glsl");
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Shader Program Init Failure", "Error", MB_OK );
#endif
		return false;
	}

	mLightSource = new LightClass();
	if(!mLightSource)
		return false;

	mLightSource->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLightSource->SetAmbientLight(0.15f, 0.15f, 0.15f, 1.0f);
	mLightSource->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLightSource->SetSpecularPower(32.0f);
	mLightSource->SetDirection(0.0f, -1.0f, 0.0f);
	mLightSource->SetPosition(0.0f, 5.0f, 0.0f);

	//Add the floor
	addNewModel("data/smallsquare.obj", "data/red.tga", "data/stone.tga");
	ModelClass* floor = (*mAllModels)[0];
	floor->setScaleFactor(Vector3(10.0f, 1.0f, 1.0f));
	floor->translate(Vector3(0.0f, -0.9f, 0.0f));
	floor->rotate(Vector3(1.0f, 0.0f, 0.0f));
	floor->setColor(Vector3(0.8039215686f, 0.78823529f, 0.78823529f));

	//Add the sphere1
	addNewModel("data/sphere.obj", "data/red.tga", "data/stone.tga");
	ModelClass* sphere1 = (*mAllModels)[1];
	sphere1->setScaleFactor(Vector3(0.5f, 0.5f, 0.5f));
	sphere1->translate(Vector3(-1.0f, 0.0f, 0.0f));

	//Add the sphere2
	addNewModel("data/sphere.obj", "data/red.tga", "data/stone.tga");
	ModelClass* sphere2 = (*mAllModels)[2];
	sphere2->setScaleFactor(Vector3(0.5f, 0.5f, 0.5f));
	sphere2->translate(Vector3(1.0f, 0.0f, 0.0f));
	sphere2->setColor(Vector3(0.0f, 0.0f, 1.0f));
	return true;
}

void ThreeDGraphicsClass::Shutdown()
{
}

bool ThreeDGraphicsClass::Render(CameraClass* camera)
{
	bool result;

	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		result = (*it)->Render(mShaderProgram, camera);
		if(!result)
		{
#ifdef _WIN32
		MessageBoxA( NULL, "Model Render Failure", "Error", MB_OK );
#endif
			return false;
		}
	}

	return true;
}

bool ThreeDGraphicsClass::addNewModel(string meshFilename, string textureFilename, string bumpmapFilename)
{
	bool result;

	ModelClass* newModel = new ModelClass();
	if(!newModel)
	{
		return false;
	}

	result = newModel->Initialize(meshFilename, textureFilename, bumpmapFilename, mLightSource);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Model Init Failure", "Error", MB_OK );
#endif
		return false;
	}

	mAllModels->push_back(newModel);

	return true;
}

bool ThreeDGraphicsClass::removeModel(string path)
{
	return true;
}

bool ThreeDGraphicsClass::scaleAll(Vector3 scaleVec)
{
	bool result;

	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		//Skip the first element
		if(it != mAllModels->begin())
		{
			(*it)->setScaleFactor(scaleVec);
		}
	}

	return true;
}

bool ThreeDGraphicsClass::translateAll(Vector3 translation)
{
	bool result;

	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		//Skip the first element
		if(it != mAllModels->begin())
		{
			(*it)->translate(translation);
		}
	}

	return true;
}

bool ThreeDGraphicsClass::rotateAll(Vector3 rotation)
{
	bool result;

	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		//Skip the first element
		if(it != mAllModels->begin())
		{
			(*it)->rotate(rotation);
		}
	}

	return true;
}

void ThreeDGraphicsClass::setOrtho(bool val)
{
	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		//Skip the first element
		if(it != mAllModels->begin())
		{
			(*it)->setOrtho(val);
		}
	}
}

void ThreeDGraphicsClass::setPost(bool val)
{
	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		//Skip the first element
		if(it != mAllModels->begin())
		{
			(*it)->setPost(val);
		}
	}
}

ModelClass* ThreeDGraphicsClass::getModel(string path)
{
	vector<ModelClass*>::iterator it;

	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		if((*it)->getMeshFilename().compare(path) == 0)
		{
			return (*it);
		}
	}

	return NULL;
}

ModelClass* ThreeDGraphicsClass::computeIntersection(Ray ray, CameraClass* camera, ModelClass* reflectedObject, Vector3* hitPosition, Vector3* normal, Vector3* viewDirection)
{
	vector<ModelClass*>::iterator it;
	float minDistance = 9999999999999.0f;
	ModelClass* closestObject = NULL;
	Vector3 closestHit = Vector3();
	Vector3 closestNormal = Vector3();
	Vector3 closestViewD = Vector3();
	bool result;
	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		if((*it) != reflectedObject)
		{
			result = (*it)->computeIntersect(ray, camera, hitPosition, normal, viewDirection);
			if(result == true)
			{
				float distance = camera->getPosition().distance(*hitPosition);
				if(distance < minDistance)
				{
					closestObject = (*it);
					closestHit = *hitPosition;
					closestNormal = *normal;
					closestViewD = *viewDirection;
				}
			}
		}
	}

	*hitPosition = closestHit;
	*normal = closestNormal ;
	*viewDirection = closestViewD;
	return closestObject;
}

bool ThreeDGraphicsClass::computeShadowIntersection(Ray ray, CameraClass* camera, ModelClass* intersectedObject)
{
	vector<ModelClass*>::iterator it;
	bool result;
	for(it = mAllModels->begin(); it != mAllModels->end(); it++)
	{
		//Intersect with itself??
		if((*it) != intersectedObject)
		{
			result = (*it)->computeShadowIntersect(ray, camera);
			if(result == true)
			{
				return (*it);
			}
		}
	}
	return NULL;
}

LightClass* ThreeDGraphicsClass::getLight()
{
	return mLightSource;
}