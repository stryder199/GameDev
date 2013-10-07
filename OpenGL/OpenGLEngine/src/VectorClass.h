#pragma once

//////////////////////
///Includes
#include <math.h>

//////////////////////
///Forward Dec
class Matrix3x3;
class Matrix4x4;

class VectorClass
{
public:
	void operator=(VectorClass &vec);

	VectorClass& operator+=(VectorClass &vec);
	VectorClass& operator+=(float val);
	VectorClass& operator+=(int val);
	VectorClass& operator-=(VectorClass &vec);
	VectorClass& operator-=(float val);
	VectorClass& operator-=(int val);
	VectorClass& operator*=(VectorClass &vec);
	VectorClass& operator*=(float val);
	VectorClass& operator*=(int val);
	
	void clamp();

	bool operator==(VectorClass &vec);
	bool operator!=(VectorClass &vec);

	float dot(VectorClass &vec);
	void normalize();
	float length();
	float poweredLength();

	virtual int getSize() = 0;
	virtual float* getElements() = 0;
};

class Vector3 : public VectorClass
{
public:
	Vector3(float x, float y, float z);
	Vector3(float xyz);
	Vector3(void);
	~Vector3(void);

	union
	{
		struct
		{
			float x, y, z;
		};
		float elements[3];
	};

	int getSize();
	float* getElements();

	Vector3 operator+(const Vector3 &vec) const;
	Vector3 operator-(const Vector3 &vec) const;
	Vector3 operator*(const Vector3 &num) const;
	Vector3 operator*(const Matrix3x3 &mat) const;
	Vector3 operator*(const float &num) const;
	Vector3 operator*(const int &num) const;

	Vector3& operator*=(Matrix3x3 &mat);

	Vector3 cross(Vector3 &vec);
	float distance(Vector3 &vec);
};

class Vector4 : public VectorClass
{
public:
	Vector4(float x, float y, float z, float w);
	Vector4(float xyzw);
	Vector4(void);
	~Vector4(void);

	union
	{
		struct
		{
			float x, y, z, w;
		};
		float elements[4];
	};

	int getSize();
	float* getElements();

	Vector4 operator+(const Vector4 &vec) const;
	Vector4 operator-(const Vector4 &vec) const;
	Vector4 operator*(const Vector4 &num) const;
	Vector4 operator*(const Matrix4x4 &mat) const;
	Vector4 operator*(const float &num) const;
	Vector4 operator*(const int &num) const;

	Vector4& operator*=(Matrix4x4 &mat);

	Vector4 cross(Vector4 &vec);
};

Vector3 cross(Vector3 &vec1, Vector3& vec2);
Vector3 vec3(Vector4 &vec);
Vector4 vec4(Vector3 &vec);