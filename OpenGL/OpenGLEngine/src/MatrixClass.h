/*Matrix of form:
*  0  4  8  12    or   0  3  6
*  1  5  9  13         1  4  7
*  2  6  10 14         2  5  8
*  3  7  11 15
*/

#pragma once

//////////////////////
///Includes
#include <string.h>
#include <math.h>

//////////////////////
///Forward Dec

class MatrixClass
{
public:
	MatrixClass& operator+=(MatrixClass &mat);
	MatrixClass& operator+=(float val);
	MatrixClass& operator-=(MatrixClass &mat);
	MatrixClass& operator-=(float val);
	MatrixClass& operator*=(MatrixClass &mat);
	MatrixClass& operator*=(float val);

	bool operator==(MatrixClass &mat);
	bool operator!=(MatrixClass &mat);

	void setIdentity();
	float det();
	void inverse();
	void cross(MatrixClass &mat);
	void rotateAroundX( float angle );
	void rotateAroundY( float angle );
	void rotateAroundZ( float angle );
	void rotateAllAxis( float xAngle, float yAngle, float zAngle);
	void scale( float xScaleFactor, float yScaleFactor);
	void scale( float xScaleFactor, float yScaleFactor, float zScaleFactor);
	void translate( float x, float y);
	void translate( float x, float y, float z);

	virtual int getSize() = 0;
	virtual float* getElements() = 0;
};

class Matrix3x3 : public MatrixClass
{
public:
	Matrix3x3(float _11, float _21, float _31, float _12, float _22, float _32, float _13, float _23, float _33);
	Matrix3x3(float allValues);
	Matrix3x3(void);
	~Matrix3x3(void);

	union{
		struct{
			float _m11, _m21, _m31, _m12, _m22, _m32, _m13, _m23, _m33;
		};
		float elements[9];
	};

	int getSize();
	float* getElements();
	const int size;

	void operator=(const Matrix3x3 &mat);

	Matrix3x3 operator+(const Matrix3x3 &mat) const;
	Matrix3x3 operator-(const Matrix3x3 &mat) const;
	Matrix3x3 operator*(const Matrix3x3 &mat) const;
	Matrix3x3 operator*(const float &num) const;

	Matrix3x3 getInverse();
	Matrix3x3 getTranspose();
	Matrix3x3 getCross(Matrix3x3 &mat);
	Matrix3x3 getMinor(Matrix3x3 &mat);

	void transpose();

	static const Matrix3x3      IDENTITY;
	static const Matrix3x3      ZERO;
};

class Matrix4x4 : public MatrixClass
{
public:
	Matrix4x4(float _11, float _21, float _31, float _41, float _12, float _22, float _32, float _42, float _13, float _23, float _33, float _43, float _14, float _24, float _34, float _44);
	Matrix4x4(float allValues);
	Matrix4x4(void);
	~Matrix4x4(void);

	union{
		struct{
			float _m11, _m21, _m31, _m41, _m12, _m22, _m32, _m42, _m13, _m23, _m33, _m43, _m14, _m24, _m34, _m44;
		};
		float elements[16];
	};

	void setPerspective(float fieldOfView, float screenAspect, float screenNear, float screenDepth);
	void setOrthoProj( float screenAspect, float screenNear, float screenDepth);
	int getSize();
	float* getElements();

	const int size;

	void operator=(const Matrix4x4 &mat);

	Matrix4x4 operator+(const Matrix4x4 &mat) const;
	Matrix4x4 operator-(const Matrix4x4 &mat) const;
	Matrix4x4 operator*(const Matrix4x4 &mat) const;
	Matrix4x4 operator*(const float &num) const;

	Matrix4x4 getInverse();
	Matrix4x4 getTranspose();
	Matrix4x4 getCross(Matrix4x4 &mat);
	Matrix4x4 getMinor(Matrix4x4 &mat);

	void transpose();

	static const Matrix4x4      IDENTITY;
	static const Matrix4x4      ZERO;
};

Matrix3x3 mat3(Matrix4x4 &matrix);