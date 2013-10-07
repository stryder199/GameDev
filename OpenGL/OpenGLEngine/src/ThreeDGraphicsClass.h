#pragma once

//////////////////////
///Includes
#include <vector>
#include <iostream>

//////////////////////
///Forward Dec
class ModelClass;
class ShaderProgram;
class Matrix4x4;
class CameraClass;
class LightClass;
class Vector3;
class Vector4;
class Ray;

using namespace std;

class ThreeDGraphicsClass
{
public:
	ThreeDGraphicsClass(void);
	~ThreeDGraphicsClass(void);

	bool Initialize();
	void Shutdown();

	bool Render(CameraClass* camera);

	bool addNewModel(string meshFilename, string textureFilename, string bumpmapFilename);
	bool removeModel(string path);
	
	bool scaleAll(Vector3 scaleFactor);
	bool translateAll(Vector3 translation);
	bool rotateAll(Vector3 rotation);
	
	void setOrtho(bool val);
	void setPost(bool val);

	ModelClass* computeIntersection(Ray ray, CameraClass* camera, ModelClass* reflectedObject, Vector3* hitPosition, Vector3* normal, Vector3* viewDirection);
	bool computeShadowIntersection(Ray ray, CameraClass* camera, ModelClass* intersectedObject);

	LightClass* getLight();
	ModelClass* getModel(string path);

private:
	vector<ModelClass*>* mAllModels;

	ShaderProgram* mShaderProgram;

	LightClass* mLightSource;
};
