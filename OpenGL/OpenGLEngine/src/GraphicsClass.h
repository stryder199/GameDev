#pragma once

//////////////////////
///Includes
#include <iostream>

//////////////////////
///Forward Dec
class ThreeDGraphicsClass;
class ShaderProgram;
class CameraClass;
class RayTracer;

class GraphicsClass
{
public:
	GraphicsClass(void);
	~GraphicsClass(void);

	bool Initialize();
	void Shutdown();

	bool Render();

	ThreeDGraphicsClass* get3DGraphics();
	void rayTraceEvent();

private:
	ThreeDGraphicsClass* mThreeDGraphics;
	CameraClass* mCamera;
	RayTracer* mRayTracer;
};
