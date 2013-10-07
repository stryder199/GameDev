#pragma once

//////////////////////
///Includes
#include <string>
#include <cmath>

#define EPSILON 0.000001
#define SCREEN_HEIGHT 700
#define SCREEN_WIDTH 700


//////////////////////
///Forward Dec
class MeshClass;
class Vector3;
class Vector4;
class Matrix4x4;
class ShaderProgram;
class TextureClass;
class LightClass;
class CameraClass;
class Ray;


const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

using namespace std;

class ModelClass
{
public:
	typedef enum
	{
		POINTS			= 0x0000,	/**< maps to GL_POINTS. */
		LINES			= 0x0001,	/**< maps to GL_LINES. */
		LINE_LOOP		= 0x0002,	/**< maps to GL_LINE_LOOP. */
		LINE_STRIP      = 0x0003,	/**< maps to GL_LINE_STRIP. */
		TRIANGLES       = 0x0004,	/**< maps to GL_TRIANGLES. */
		TRIANGLE_STRIP  = 0x0005,	/**< maps to GL_TRIANGLE_STRIP. */
		TRIANGLE_FAN    = 0x0006,	/**< maps to GL_TRIANGLE_FAN. */

		QUADS,						/**< compatibility for quads. we are triangulating them. */
		QUAD_STRIP					/**< compatibility for quad strips. we are triangulating them. */
	}
	GeometryPrimitiveType;

	typedef enum
	{
		SPHERE = 0x0000,
		FLOOR = 0x0001,
		OTHER = 0x0002
	}
	ObjectType;

	ModelClass();
	~ModelClass(void);

	bool Initialize(string meshFilename, string textureFilename, string bumpmapFilename,  LightClass* lightSource);
	void Shutdown();

	bool Render(ShaderProgram* shaderProgram, CameraClass* camera);

	void translate(Vector3 m_Translation);
	void setScaleFactor(Vector3 scaleFactor);
	void rotate(Vector3 rotation);
	void setOrtho(bool val);
	void setPost(bool val);
	void setColor(Vector3 newColor);

	string getName();
	string getMeshFilename();
	Vector3 getColor();

	bool computeIntersect(Ray ray, CameraClass* camera, Vector3* hitPoint, Vector3* normal, Vector3* viewDirection);
	bool computeShadowIntersect(Ray ray, CameraClass* camera);
	bool computeTriangleIntersect(Ray ray, Vector4 v1, Vector4 v2, Vector4 v3);
	bool computeSphereIntersect(Ray ray, Vector3* intersectPoint1, Vector3* intersectPoint2, Vector3* nromal);
	bool computePlaneIntersect(Ray ray, Vector3 planeNormal, Vector3 planeOrigin, Vector3* intersectPoint);
	
	ObjectType  objType;

private:
	void initOpenGLBuffers();
	string parseFilename();

	MeshClass* mMesh;
	TextureClass* mTexture;

	Matrix4x4 *mPerspectiveMatrix, *mOrthoMatrix;

	GeometryPrimitiveType mPrimitive;

	string mMeshFilename, mTexFilename, mName;

	

	unsigned int mColorBuffer;				/**< handle for the color buffer. */
	unsigned int mNormalBuffer;				/**< handle for the normal buffer. */
	unsigned int mTexCoordBuffer;			/**< handle for the texture coordinate buffer. */
	unsigned int mVertexBuffer;				/**< handle for the vertex buffer. */
	unsigned int mIndexBuffer;
	unsigned int mBinormalBuffer;
	unsigned int mTangentBuffer;

	unsigned int mVertexArray;				/**< the VAO handle. */

	unsigned int radius;

	bool mPost, mOrtho;

	Vector3 *mScale, *mTranslation, *mRotation;           /**< the current scalefactor of the geometry. */

	LightClass* mLightSource;

	Vector3 *color;
};
