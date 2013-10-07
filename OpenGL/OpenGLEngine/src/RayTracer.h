#pragma once

#define FRAME_WIDTH 700
#define FRAME_HEIGHT 700
#define MAX_SAMPLE 16
#define MAX_RAY_BOUNCE 4

#include "MatrixClass.h"
#include "VectorClass.h"

class Ray;
class ThreeDGraphicsClass;
class CameraClass;
class LightClass;
class ModelClass;

class RayTracer
{
public:
	struct Pixel{
		float r, g, b;
	};

	RayTracer(void);
	~RayTracer(void);

	bool Initialize(ThreeDGraphicsClass* models, CameraClass* camera, LightClass* light);
	void Shutdown();

	bool RunRayTrace();
	Pixel TracePixel(Ray* ray, ModelClass* reflectedObject, int depth);
	void computeRayDir(int x, int y, Ray* ray);

private:
	Pixel frame[FRAME_HEIGHT][FRAME_WIDTH];
	ThreeDGraphicsClass* mThreeDGraphics;
	LightClass* mLight;
	CameraClass* mCamera;
	Matrix4x4 mProjectionMatrix, mViewMatrix, mInverseViewMatrix, mWorldMatrix, mTranslateMatrix, mInverseWorldMatrix;
	Vector3 mCameraOrigin;

	bool saveImage();
};

float clamp(float &x);