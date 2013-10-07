#pragma once

//////////////////////
///Includes

//////////////////////
///Forward Dec
class Vector3;
class Vector4;

using namespace std;

class LightClass
{
public:
	LightClass(void);
	~LightClass(void);

	void Shutdown();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetAmbientLight(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float pow);
	void SetPosition(float x, float y, float z);

	Vector3* getDirection();
	Vector4* getDiffuseColor();
	Vector4* getAmbientLight();
	Vector4* getSpecularColor();
	Vector3* getPosition();
	float getSpecularPower();

private:
	Vector4 *mDiffuseColor;
	Vector3 *mDirection;
	Vector4 *mAmbientLight;
	Vector4 *mSpecularColor;
	Vector3 *mPosition;
	float mSpecularPower;
};
