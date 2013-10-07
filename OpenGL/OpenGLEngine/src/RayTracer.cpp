#include "RayTracer.h"
#include "Ray.h"
#include "ThreeDGraphicsClass.h"
#include "LightClass.h"
#include "ModelClass.h"
#include "VectorClass.h"
#include "MatrixClass.h"
#include <qplugin.h>
#include <QPictureFormatPlugin>
#include <qfile.h>
#include <qpixmap.h>
#include <qimage.h>
#include "CameraClass.h"
#include <Windows.h>
#include <qimagewriter.h>

RayTracer::RayTracer(void)
{
}


RayTracer::~RayTracer(void)
{
}

bool RayTracer::Initialize(ThreeDGraphicsClass* models, CameraClass* camera, LightClass* light)
{
	bool result;
	
	mThreeDGraphics = models;
	mLight = light;
	mCamera = camera;

	float fieldOfView = 3.14159265358979323846f / 4.0f;
	float screenAspect = (float)FRAME_WIDTH / (float)FRAME_HEIGHT;
	mProjectionMatrix.setPerspective(fieldOfView, screenAspect, (float)SCREEN_NEAR, (float)SCREEN_DEPTH);

	mViewMatrix = camera->GetViewMatrix();
	mInverseViewMatrix = mViewMatrix.getInverse();

	mCameraOrigin = camera->getPosition();

	Pixel zeroPix = {0.0f, 0.0f, 0.0f};

	for(int i=0; i<FRAME_HEIGHT; i++)
	{
		for(int j=0; j<FRAME_WIDTH; j++)
		{
			frame[i][j] = zeroPix;
		}
	}


	return true;
}

void RayTracer::Shutdown()
{
}

bool RayTracer::RunRayTrace()
{
	for(int y=0; y<FRAME_HEIGHT; y++)
	{
		for(int x=0; x<FRAME_WIDTH; x++)
		{
			Ray ray;
			computeRayDir(x, y, &ray);
			frame[y][x] = TracePixel(&ray, NULL, 0);
		}
	}

	if(!saveImage())
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Mesh Init Failure", "Error", MB_OK );
#endif
		cerr << "Mesh Init Failure" << endl;
		return false;
	}

	return true;
}

RayTracer::Pixel RayTracer::TracePixel(Ray* ray, ModelClass* reflectedObject, int depth)
{
	Pixel finalColor = {0.0f, 0.0f, 0.0f};
	ModelClass* intersectObject = NULL;
	Vector3 normal, viewDirection, hitPoint;

	if(depth == MAX_RAY_BOUNCE)
	{
		//You hit the max amount of reflections
		finalColor.r = 0.0f;
		finalColor.g = 0.0f;
		finalColor.b = 0.0f;
		return finalColor;
	}

	intersectObject = mThreeDGraphics->computeIntersection(*ray, mCamera, reflectedObject, &hitPoint, &normal, &viewDirection);

	if(intersectObject == NULL)
	{
		//Didnt intersect anything
		finalColor.r = 0.0f;
		finalColor.g = 0.0f;
		finalColor.b = 0.0f;
	}
	else{
		if(depth == 1)
		{
			int i = 0;
		}

		//Compute illum
		Ray shadowRay;
		bool inShadow = false;
		shadowRay.origin = hitPoint;
		shadowRay.direction = *mLight->getPosition() - hitPoint;
		inShadow = mThreeDGraphics->computeShadowIntersection(shadowRay, mCamera, intersectObject);
		
		if(!inShadow)
		{
			//Do lighting calc
			float diffuseCoefficient;
			Vector3 intersectObjectColor, lightDir, reflection;
			Vector4 specularColor, finalLightColor, outputColor;

			intersectObjectColor = intersectObject->getColor();

			//Init the specular color
			specularColor = Vector4();
		
			//Minimum brightness that the pixel must be set to
			outputColor = *(mLight->getAmbientLight());

			//Invert the light direction
			lightDir = (*mLight->getDirection())*-1;

			//Calc the amount of light on this pixel
			diffuseCoefficient = normal.dot(lightDir);
			diffuseCoefficient = clamp(diffuseCoefficient);

			if(diffuseCoefficient > 0.0f)
			{
				//Find the final diffuse light color if the object isnt pointing away from the light source
				outputColor += (*mLight->getDiffuseColor() * diffuseCoefficient);
				outputColor.clamp();

				//Calculate the reflection vector vased on the light intensity, normal vector and light direction
				reflection = normal*(2*diffuseCoefficient);
				reflection = reflection - lightDir;
				reflection.normalize();

				//Determine the amount of specular light based on the reflection vector, viewing direction and specular power
				float specularCoefficient = reflection.dot(viewDirection);
				specularCoefficient = clamp(specularCoefficient);
				specularCoefficient = pow(specularCoefficient, mLight->getSpecularPower());
				specularColor = *(mLight->getSpecularColor())*specularCoefficient;
			}
			//Find the final color by multing the lightColor with the texture color
			outputColor = outputColor*vec4(intersectObjectColor);

			//Add the specular component to the final color
			outputColor = outputColor + specularColor;

			Pixel reflectionColor = {0.0f, 0.0f, 0.0f};
			if(depth < MAX_RAY_BOUNCE)
			{
				//Compute reflection
				Ray reflectionRay;
				float c1 = -1*normal.dot(ray->direction);
				reflectionRay.direction = ray->direction + ((normal*2)*c1);
				reflectionRay.origin = hitPoint;
				reflectionColor = TracePixel(&reflectionRay, intersectObject, depth+1);
			}

			outputColor = outputColor + Vector4(reflectionColor.r/mLight->getSpecularPower(), reflectionColor.g/mLight->getSpecularPower(), reflectionColor.b/mLight->getSpecularPower(), 1.0f);
			outputColor.clamp();

			//Convert to pixel format
			finalColor.r = outputColor.x;
			finalColor.g = outputColor.y;
			finalColor.b = outputColor.z;
		}
		else{
			//In shadow
			finalColor.r = 0.0f;
			finalColor.g = 0.0f;
			finalColor.b = 0.0f;
		}
	}

	return finalColor;
}

void RayTracer::computeRayDir(int x, int y, Ray* ray)
{
	float pointX, pointY;
	Vector3 direction, origin, rayOrigin, rayDirection;
	bool intersect, result;


	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)x) / (float)FRAME_WIDTH) - 1.0f;
	pointY = (((2.0f * (float)y) / (float)FRAME_HEIGHT) - 1.0f) * -1.0f;
		

	pointX = pointX / mProjectionMatrix._m11;
	pointY = pointY / mProjectionMatrix._m22;


	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * mInverseViewMatrix._m11) + (pointY * mInverseViewMatrix._m21) + mInverseViewMatrix._m31;
	direction.y = (pointX * mInverseViewMatrix._m12) + (pointY * mInverseViewMatrix._m22) + mInverseViewMatrix._m32;
	direction.z = (pointX * mInverseViewMatrix._m13) + (pointY * mInverseViewMatrix._m23) + mInverseViewMatrix._m33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = mCameraOrigin;

	ray->origin = origin;
	ray->direction = direction;
}

bool RayTracer::saveImage()
{
	QFile file("tracedImage.ppm");
	QImage image(FRAME_WIDTH, FRAME_HEIGHT, QImage::Format_RGB32);
	
	if(file.exists())
	{
		file.remove();
	}

	if(!file.open(QIODevice::WriteOnly))
	{
#ifdef _WIN32
		MessageBoxA( NULL, "Error opening tracer file", "Error", MB_OK );
#endif
		cerr << "Error opening tracer file" << endl;
		return false;
	}

	for ( uint j = 0; j <FRAME_WIDTH; j++ )
	{
		for ( uint i = 0; i <FRAME_HEIGHT; i++ )
		{
			QRgb pixel;
			pixel = qRgb( int(frame[j][i].r*255), int(frame[j][i].g*255) , int(frame[j][i].b*255) );
			image.setPixel( i, j, pixel );
		}
	}
	QImageWriter writer("tracedImage.ppm", "ppm");
	if(!writer.write(image))
	{
		string writeError = writer.errorString().toUtf8().constData();
		string error = "Tracer save error, " + writeError;
#ifdef _WIN32
		MessageBoxA( NULL, error.c_str(), "Error", MB_OK );
#endif
		cerr << "Tracer save error" << endl;
		return false;
	}
	return true;
}

float clamp(float &x)
{
	if(	x < 0.0f )
	{
		return 0.0f;
	}
	if( x > 1.0f )
	{
		return 1.0f;
	}

	return x;
}