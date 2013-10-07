#include "VectorClass.h"
#include "MatrixClass.h"

void VectorClass::operator=(VectorClass &vec)
{
	if(getSize() != vec.getSize())
	{
		return;
	}

	getElements()[0] = vec.getElements()[0];
	getElements()[1] = vec.getElements()[1];
	getElements()[2] = vec.getElements()[2];

	if(getSize() == 4 && vec.getSize() == 4)
	{
		getElements()[3] = vec.getElements()[3];
	}
}

VectorClass& VectorClass::operator+=(VectorClass &vec)
{
	getElements()[0] += vec.getElements()[0];
	getElements()[1] += vec.getElements()[1];
	getElements()[2] += vec.getElements()[2];

	if(getSize() == 4 && vec.getSize() == 4)
	{
		getElements()[3] += vec.getElements()[3];
	}

	return *this;
}

VectorClass& VectorClass::operator+=(float val)
{
	getElements()[0] += val;
	getElements()[1] += val;
	getElements()[2] += val;

	if(getSize() == 4)
	{
		getElements()[3] += val;
	}

	return *this;
}

VectorClass& VectorClass::operator+=(int val)
{
	getElements()[0] += val;
	getElements()[1] += val;
	getElements()[2] += val;

	if(getSize() == 4)
	{
		getElements()[3] += val;
	}

	return *this;
}

VectorClass& VectorClass::operator-=(VectorClass &vec)
{
	getElements()[0] -= vec.getElements()[0];
	getElements()[1] -= vec.getElements()[1];
	getElements()[2] -= vec.getElements()[2];

	if(getSize() == 4 && vec.getSize() == 4)
	{
		getElements()[3] -= vec.getElements()[3];
	}

	return *this;
}
VectorClass& VectorClass::operator-=(float val)
{
	getElements()[0] -= val;
	getElements()[1] -= val;
	getElements()[2] -= val;

	if(getSize() == 4)
	{
		getElements()[3] -= val;
	}

	return *this;
}

VectorClass& VectorClass::operator-=(int val)
{
	getElements()[0] -= val;
	getElements()[1] -= val;
	getElements()[2] -= val;

	if(getSize() == 4)
	{
		getElements()[3] -= val;
	}

	return *this;
}

VectorClass& VectorClass::operator*=(VectorClass &vec)
{
	getElements()[0] *= vec.getElements()[0];
	getElements()[1] *= vec.getElements()[1];
	getElements()[2] *= vec.getElements()[2];

	if(getSize() == 4 && vec.getSize() == 4)
	{
		getElements()[3] *= vec.getElements()[3];
	}

	return *this;
}
VectorClass& VectorClass::operator*=(float val)
{
	getElements()[0] *= val;
	getElements()[1] *= val;
	getElements()[2] *= val;

	if(getSize() == 4)
	{
		getElements()[3] *= val;
	}

	return *this;
}

VectorClass& VectorClass::operator*=(int val)
{
	getElements()[0] *= val;
	getElements()[1] *= val;
	getElements()[2] *= val;

	if(getSize() == 4)
	{
		getElements()[3] *= val;
	}

	return *this;
}

bool VectorClass::operator==(VectorClass &vec)
{
	if(getSize() == 3 && vec.getSize() == 3)
	{
		return (getElements()[0] == vec.getElements()[0] && getElements()[1] == vec.getElements()[1] && getElements()[2] == vec.getElements()[2]);
	}
	if(getSize() == 4 && vec.getSize() == 4)
	{
		return (getElements()[0] == vec.getElements()[0] && getElements()[1] == vec.getElements()[1] && getElements()[2] == vec.getElements()[2] && getElements()[3] == vec.getElements()[3]);
	}
	return false;
}

bool VectorClass::operator!=(VectorClass &vec)
{
	if(getSize() == 3 && vec.getSize() == 3)
	{
		return !(getElements()[0] == vec.getElements()[0] && getElements()[1] == vec.getElements()[1] && getElements()[2] == vec.getElements()[2]);
	}
	if(getSize() == 4 && vec.getSize() == 4)
	{
		return !(getElements()[0] == vec.getElements()[0] && getElements()[1] == vec.getElements()[1] && getElements()[2] == vec.getElements()[2] && getElements()[3] == vec.getElements()[3]);
	}
	return true;
}

void VectorClass::clamp()
{
	if(	getElements()[0] < 0.0f )
	{
		getElements()[0] = 0.0f;
	}
	if( getElements()[0] > 1.0f )
	{
		getElements()[0] = 1.0f;
	}

	if(	getElements()[1] < 0.0f )
	{
		getElements()[1] = 0.0f;
	}
	if( getElements()[1] > 1.0f )
	{
		getElements()[1] = 1.0f;
	}

	if(	getElements()[2] < 0.0f )
	{
		getElements()[2] = 0.0f;
	}
	if( getElements()[2] > 1.0f )
	{
		getElements()[2] = 1.0f;
	}

	if(getSize() == 4)
	{
		if(	getElements()[3] < 0.0f )
		{
			getElements()[3] = 0.0f;
		}
		if( getElements()[3] > 1.0f )
		{
			getElements()[3] = 1.0f;
		}
	}
}

float VectorClass::dot(VectorClass &vec)
{
	float total = 0.0f;

	for(int i=0; i<getSize(); i++)
	{
		total += (vec.getElements()[i] * getElements()[i]);
	}

	return total;
}
void VectorClass::normalize()
{
	float thisLength = length();
	getElements()[0] /= thisLength;
	getElements()[1] /= thisLength;
	getElements()[2] /= thisLength;

	if(getSize() == 4)
	{
		getElements()[3] /= thisLength;
	}
}

float VectorClass::length()
{
	return sqrtf(poweredLength());
}

float VectorClass::poweredLength()
{
	float total = 0.0f;

	for(int i=0; i<getSize(); i++)
	{
		total += (getElements()[i] * getElements()[i]);
	}

	return total;
}

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float xyz)
{
	x = xyz;
	y = xyz;
	z = xyz;
}

Vector3::Vector3(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}

Vector3::~Vector3()
{
}

int Vector3::getSize()
{
	return 3;
}

float* Vector3::getElements()
{
	return elements;
}

Vector3 Vector3::operator+(const Vector3 &vec) const
{
	return Vector3(elements[0] + vec.elements[0], elements[1] + vec.elements[1], elements[2] + vec.elements[2]);
}

Vector3 Vector3::operator-(const Vector3 &vec) const
{
	return Vector3(elements[0] - vec.elements[0], elements[1] - vec.elements[1], elements[2] - vec.elements[2]);
}

Vector3 Vector3::operator*(const Vector3 &vec) const
{
	return Vector3(elements[0] * vec.elements[0], elements[1] * vec.elements[1], elements[2] * vec.elements[2]);
}

Vector3 Vector3::operator*(const Matrix3x3 &mat) const
{
	Vector3 result;
	float xtemp = x*mat._m11 + y*mat._m12 + z*mat._m13;
	float ytemp = x*mat._m21 + y*mat._m22 + z*mat._m23;
	float ztemp = x*mat._m31 + y*mat._m32 + z*mat._m33;
	result.x=xtemp;
	result.y=ytemp;
	result.z=ztemp;

	return result;
}

Vector3 Vector3::operator*(const float &num) const
{
	return Vector3(elements[0] * num, elements[1] * num, elements[2] * num);
}

Vector3 Vector3::operator*(const int &num) const
{
	return Vector3(elements[0] * num, elements[1] * num, elements[2] * num);
}

Vector3& Vector3::operator*=(Matrix3x3 &mat) 
{
	float xtemp = x*mat._m11 + y*mat._m12 + z*mat._m13;
	float ytemp = x*mat._m21 + y*mat._m22 + z*mat._m23;
	float ztemp = x*mat._m31 + y*mat._m32 + z*mat._m33;
	x=xtemp;
	y=ytemp;
	z=ztemp;

	return *this;
}

Vector3 Vector3::cross(Vector3 &vec)
{
	Vector3 result;
	result.x = (y * vec.z) - (z * vec.y);
	result.y = (z * vec.x) - (x * vec.z);
	result.z = (x * vec.y) - (y * vec.x);
	return result;
}

float Vector3::distance(Vector3 &vec)
{
	float deltaX = x - vec.x;
	float deltaY = y - vec.y;
	float deltaZ = z - vec.z;
	return sqrtf(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
}

Vector4::Vector4()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector4::Vector4( float xyzw )
{
	x = xyzw;
	y = xyzw;
	z = xyzw;
	w = xyzw;
}

Vector4::Vector4( float X, float Y, float Z, float W )
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

Vector4::~Vector4()
{
}

int Vector4::getSize()
{
	return 4;
}

float* Vector4::getElements()
{
	return elements;
}

Vector4 Vector4::operator+(const Vector4 &vec) const
{
	return Vector4(elements[0] + vec.elements[0], elements[1] + vec.elements[1], elements[2] + vec.elements[2], elements[3] + vec.elements[3]);
}

Vector4 Vector4::operator-(const Vector4 &vec) const
{
	return Vector4(elements[0] - vec.elements[0], elements[1] - vec.elements[1], elements[2] - vec.elements[2], elements[3] - vec.elements[3]);
}

Vector4 Vector4::operator*(const Vector4 &vec) const
{
	return Vector4(elements[0] * vec.elements[0], elements[1] * vec.elements[1], elements[2] * vec.elements[2], elements[3] * vec.elements[3]);
}

Vector4 Vector4::operator*(const Matrix4x4 &mat) const
{
	Vector4 result;
	float xtemp = x*mat._m11 + y*mat._m12 + z*mat._m13 + w*mat._m14;
	float ytemp = x*mat._m21 + y*mat._m22 + z*mat._m23 + w*mat._m24;
	float ztemp = x*mat._m31 + y*mat._m32 + z*mat._m33 + w*mat._m34;
	float wtemp = x*mat._m41 + y*mat._m42 + z*mat._m43 + w*mat._m44;
	result.x=xtemp;
	result.y=ytemp;
	result.z=ztemp;
	result.w=wtemp;

	return result;
}

Vector4 Vector4::operator*(const float &num) const
{
	return Vector4(elements[0] * num, elements[1] * num, elements[2] * num, elements[3] * num);
}

Vector4 Vector4::operator*(const int &num) const
{
	return Vector4(elements[0] * num, elements[1] * num, elements[2] * num, elements[3] * num);
}

Vector4 Vector4::cross(Vector4 &vec)
{
	return Vector4();
}

Vector4& Vector4::operator*=(Matrix4x4 &mat) 
{
	float xtemp = x*mat._m11 + y*mat._m12 + z*mat._m13 + w*mat._m14;
	float ytemp = x*mat._m21 + y*mat._m22 + z*mat._m23 + w*mat._m24;
	float ztemp = x*mat._m31 + y*mat._m32 + z*mat._m33 + w*mat._m34;
	float wtemp = x*mat._m41 + y*mat._m42 + z*mat._m43 + w*mat._m44;
	x=xtemp;
	y=ytemp;
	z=ztemp;
	w=wtemp;

	return *this;
}

Vector3 cross(Vector3 &vec1, Vector3& vec2)
{
	Vector3 result;
	result.x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
	result.y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
	result.z = (vec1.x * vec2.y) - (vec1.y * vec2.x);
	return result;
}

Vector3 vec3(Vector4 &vec)
{
	Vector3 result;
	result.x = vec.x;
	result.y = vec.y;
	result.z = vec.z;
	return result;
}

Vector4 vec4(Vector3 &vec)
{
	Vector4 result;
	result.x = vec.x;
	result.y = vec.y;
	result.z = vec.z;
	result.w = 1.0f;
	return result;
}