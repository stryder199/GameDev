#include "GraphicsClass.h"
#include "ThreeDGraphicsClass.h"
#include "cameraclass.h"
#include "MatrixClass.h"
#include "RayTracer.h"

#ifdef _WIN32
#include <Windows.h>
#endif

GraphicsClass::GraphicsClass(void)
{
	mThreeDGraphics = 0;
	mCamera = 0;
	mRayTracer = 0;
}

GraphicsClass::~GraphicsClass(void)
{
}

bool GraphicsClass::Initialize()
{
	bool result;

	mCamera = new CameraClass();
	if(!mCamera)
	{
		cerr << "Camera Obj Creation Failure" <<endl;
		return false;
	}

	mCamera->SetPosition(0.0f, 0.0f, -2.5f);

	mThreeDGraphics = new ThreeDGraphicsClass();
	if(!mThreeDGraphics)
	{
		cerr << "3D graphics Obj Creation Failure" << endl;
		return false;
	}

	result = mThreeDGraphics->Initialize();
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "3D Grpahics Init Failure", "Error", MB_OK );
#endif
		return false;
	}

	mRayTracer = new RayTracer();
	if(!mRayTracer)
	{
		cerr << "Ray Tracer Obj Creation Failure" << endl;
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
}

bool GraphicsClass::Render()
{
	bool result;

	mCamera->Render();

	result = mThreeDGraphics->Render(mCamera);
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "3D Grpahics Render Failure", "Error", MB_OK );
#endif
		cerr << "3D Graphics Render Failure" << endl;
		return false;
	}

	return true;
}

ThreeDGraphicsClass* GraphicsClass::get3DGraphics()
{
	return mThreeDGraphics;
}

void GraphicsClass::rayTraceEvent()
{
	bool result;
	result = mRayTracer->Initialize(mThreeDGraphics, mCamera, mThreeDGraphics->getLight());
	if(!result)
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Ray Tracer Init Failure", "Error", MB_OK );
#endif
	}

	mRayTracer->RunRayTrace();

}