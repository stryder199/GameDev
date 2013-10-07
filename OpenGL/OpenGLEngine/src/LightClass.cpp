#include "LightClass.h"
#include "VectorClass.h"

LightClass::LightClass(void)
{
	mDiffuseColor = 0;
	mDirection = 0;
	mAmbientLight = 0;
	mSpecularColor = 0;
	mSpecularPower = 0.0f;
	mPosition = 0;
}

LightClass::~LightClass(void)
{
}

void LightClass::Shutdown()
{

}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	if(!mDiffuseColor)
	{
		mDiffuseColor = new Vector4();
	}
	mDiffuseColor->x = red;
	mDiffuseColor->y = green;
	mDiffuseColor->z = blue;
	mDiffuseColor->w = alpha;
}

void LightClass::SetAmbientLight(float red, float green, float blue, float alpha)
{
	if(!mAmbientLight)
	{
		mAmbientLight = new Vector4();
	}
	mAmbientLight->x = red;
	mAmbientLight->y = green;
	mAmbientLight->z = blue;
	mAmbientLight->w = alpha;
}

void LightClass::SetDirection(float x, float y, float z)
{
	if(!mDirection)
	{
		mDirection = new Vector3();
	}
	mDirection->x = x;
	mDirection->y = y;
	mDirection->z = z;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	if(!mSpecularColor)
	{
		mSpecularColor = new Vector4();
	}
	mSpecularColor->x = red;
	mSpecularColor->y = green;
	mSpecularColor->z = blue;
	mSpecularColor->w = alpha;
}

void LightClass::SetSpecularPower(float pow)
{
	mSpecularPower = pow;
}

void LightClass::SetPosition(float x, float y, float z)
{
	if(!mPosition)
	{
		mPosition = new Vector3();
	}
	mPosition->x = x;
	mPosition->y = y;
	mPosition->z = z;
}

Vector3* LightClass::getDirection()
{
	return mDirection;
}

Vector4* LightClass::getDiffuseColor()
{
	return mDiffuseColor;
}

Vector4* LightClass::getAmbientLight()
{
	return mAmbientLight;
}

Vector4* LightClass::getSpecularColor()
{
	return mSpecularColor;
}

float LightClass::getSpecularPower()
{
	return mSpecularPower;
}

Vector3* LightClass::getPosition()
{
	return mPosition;
}