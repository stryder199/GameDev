#include "MatrixClass.h"

void MatrixClass::scale	(float scaleX, float scaleY)
{
	getElements()[0] *= scaleX;
	getElements()[4] *= scaleY;
}

void MatrixClass::scale	(float scaleX, float scaleY, float scaleZ)
{
	getElements()[0] *= scaleX;
	getElements()[5] *= scaleY;
	getElements()[10] *= scaleZ;
}

void MatrixClass::translate(float x, float y, float z)
{
	getElements()[12] += x;
	getElements()[13] += y;
	getElements()[14] += z;
}

void MatrixClass::rotateAroundX(float angle)
{
	if(getSize() == 9)
	{
		getElements()[5] = cosf(angle);
		getElements()[6] = sinf(angle);
		getElements()[9] = -1*sinf(angle);
		getElements()[10] = cosf(angle);
	}
	else if(getSize() == 16)
	{
		getElements()[4] = cosf(angle);
		getElements()[5] = sinf(angle);
		getElements()[7] = -1*sinf(angle);
		getElements()[8] = cosf(angle);
	}
}

void MatrixClass::rotateAroundY(float angle)
{
	getElements()[0] = cosf(angle);
	getElements()[2] = -1*sinf(angle);
	if(getSize() == 9)
	{
		getElements()[6] = sinf(angle);
		getElements()[8] = cosf(angle);
	}
	else if(getSize() == 16)
	{
		getElements()[8] = sinf(angle);
		getElements()[10] = cosf(angle);
	}
}

void MatrixClass::rotateAroundZ(float angle)
{
	getElements()[0] = cosf(angle);
	getElements()[1] = sinf(angle);
	if(getSize() == 9)
	{
		getElements()[3] = -1*sinf(angle);
		getElements()[4] = cosf(angle);
	}
	else if(getSize() == 16)
	{
		getElements()[4] = -1*sinf(angle);
		getElements()[5] = cosf(angle);
	}
}

void MatrixClass::rotateAllAxis(float pitch, float yaw, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;

	// Get the cosine and sin of the yaw, pitch, and roll.
	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	// Calculate the yaw, pitch, roll rotation matrix.
	getElements()[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	getElements()[1] = (sRoll * cPitch);
	getElements()[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);

	if (getSize() == 9)
	{
		getElements()[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
		getElements()[4] = (cRoll * cPitch);
		getElements()[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);

		getElements()[6] = (cPitch * sYaw);
		getElements()[7] = -sPitch;
		getElements()[8] = (cPitch * cYaw);
	}
	else if(getSize() == 16)
	{
		getElements()[4] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
		getElements()[5] = (cRoll * cPitch);
		getElements()[6] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);

		getElements()[8] = (cPitch * sYaw);
		getElements()[9] = -sPitch;
		getElements()[10] = (cPitch * cYaw);
	}
}

void MatrixClass::setIdentity()
{
	if(getSize() == 9)
	{
		memcpy(getElements(), Matrix3x3::IDENTITY.elements, sizeof(float)*getSize());
	}
	else if(getSize() == 16)
	{
		memcpy(getElements(), Matrix4x4::IDENTITY.elements, sizeof(float)*getSize());
	}
}

MatrixClass& MatrixClass::operator+=(MatrixClass &mat)
{
	if(getSize() != mat.getSize())
	{
		return Matrix3x3();
	}

	for(int i=0; i<getSize(); i++)
	{
		getElements()[i] += mat.getElements()[i];
	}

	return *this;
}

MatrixClass& MatrixClass::operator+=(float val)
{
	for(int i=0; i<getSize(); i++)
	{
		getElements()[i] += val;
	}

	return *this;
}

MatrixClass& MatrixClass::operator-=(MatrixClass &mat)
{
	if(getSize() != mat.getSize())
	{
		return Matrix3x3();
	}

	for(int i=0; i<getSize(); i++)
	{
		getElements()[i] -= mat.getElements()[i];
	}

	return *this;
}

MatrixClass& MatrixClass::operator-=(float val)
{
	for(int i=0; i<getSize(); i++)
	{
		getElements()[i] -= val;
	}

	return *this;
}

MatrixClass& MatrixClass::operator*=(MatrixClass &mat)
{
	Matrix3x3 temp3 = Matrix3x3();
	Matrix4x4 temp4 = Matrix4x4();

	if(getSize() != mat.getSize())
	{
		return Matrix3x3();
	}

	if(getSize() == 9)
	{
		for(int row=0; row<3; row++)
		{
			for(int col=0; col<3; col++)
			{
				for(int add=0; add<3; add++)
				{
					temp3.getElements()[row*3 + col] += getElements()[row + add*3] * mat.getElements()[3*col + add];
				}
			}
		}

		for(int row=0; row<3; row++)
		{
			for(int col=0; col<3; col++)
			{
				getElements()[row*3 + col] = temp3.getElements()[row*3 + col];
			}
		}
	}
	else if(getSize() == 16)
	{
		for(int row=0; row<4; row++)
		{
			for(int col=0; col<4; col++)
			{
				for(int add=0; add<4; add++)
				{
					temp4.getElements()[col*4 + row] += getElements()[row + add*4] * mat.getElements()[4*col + add];
				}
			}
		}

		for(int row=0; row<4; row++)
		{
			for(int col=0; col<4; col++)
			{
				getElements()[row*4 + col] = temp4.getElements()[row*4 + col];
			}
		}
	}

	return *this;
}

MatrixClass& MatrixClass::operator*=(float val)
{
	for(int i=0; i<getSize(); i++)
	{
		getElements()[i] *= val;
	}

	return *this;
}

bool MatrixClass::operator==(MatrixClass &mat)
{
	if(getSize() != mat.getSize())
	{
		return false;
	}

	for(int i=0; i<getSize(); i++)
	{
		if(getElements()[i] != mat.getElements()[i])
		{
			return false;
		}
	}

	return true;
}

bool MatrixClass::operator!=(MatrixClass &mat)
{
	if(getSize() != mat.getSize())
	{
		return true;
	}

	for(int i=0; i<getSize(); i++)
	{
		if(getElements()[i] != mat.getElements()[i])
		{
			return true;
		}
	}

	return false;
}

float MatrixClass::det()
{
	return 0.0f;
}

void MatrixClass::inverse()
{
}

void MatrixClass::cross(MatrixClass &mat)
{
}

Matrix3x3::Matrix3x3(float _11, float _21, float _31, float _12, float _22, float _32, float _13, float _23, float _33) : size(9)
{
	_m11 = _11;
	_m21 = _21;
	_m31 = _31;
	_m12 = _12;
	_m22 = _22;
	_m32 = _32;
	_m13 = _13;
	_m23 = _23;
	_m33 = _33;
}
Matrix3x3::Matrix3x3(float allValues) : size(9)
{
	for(int i=0; i<size; i++)
	{
		elements[i] = allValues;
	}
}

Matrix3x3::Matrix3x3() : size(9)
{
	memcpy(elements, Matrix3x3::ZERO.elements, sizeof(float)*size);
}

Matrix3x3::~Matrix3x3()
{
}

int Matrix3x3::getSize()
{
	return 9;
}

float* Matrix3x3::getElements()
{
	return elements;
}

void Matrix3x3::operator=(const Matrix3x3 &mat)
{
	for(int i=0; i<size; i++)
	{
		elements[i] = mat.elements[i];
	}
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3 &mat) const
{
	Matrix3x3 result;
	for(int i=0; i<size; i++)
	{
		result.elements[i] = elements[i] + mat.elements[i];
	}
	return result;
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3 &mat) const
{
	Matrix3x3 result;
	for(int i=0; i<size; i++)
	{
		result.elements[i] = elements[i] - mat.elements[i];
	}
	return result;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &mat) const
{
	Matrix3x3 temp = Matrix3x3();

	for(int row=0; row<3; row++)
	{
		for(int col=0; col<3; col++)
		{
			for(int add=0; add<3; add++)
			{
				temp.elements[row*3 + col] += elements[row + add*3] * mat.elements[3*col + add];
			}
		}
	}

	return temp;
}

Matrix3x3 Matrix3x3::operator*(const float &num) const
{
	Matrix3x3 result;
	for(int i=0; i<size; i++)
	{
		result.elements[i] = elements[i] * num;
	}
	return result;
}

Matrix3x3 Matrix3x3::getInverse()
{
	return Matrix3x3();
}

Matrix3x3 Matrix3x3::getTranspose()
{
	Matrix3x3 transpose;

	for(int col=0; col<3; col++)
	{
		for(int row=0; row<3; row++)
		{
			transpose.elements[col*3 + row] = elements[row*3 + col];
		}
	}

	return transpose;
}

Matrix3x3 Matrix3x3::getCross(Matrix3x3 &mat)
{
	return Matrix3x3();
}

Matrix3x3 Matrix3x3::getMinor(Matrix3x3 &mat)
{
	return Matrix3x3();
}

void Matrix3x3::transpose()
{
	//Matrix3x3 transpose = getTranspose();

	//*this = transpose;
}

const Matrix3x3 Matrix3x3::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

const Matrix3x3 Matrix3x3::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

Matrix4x4::Matrix4x4(float _11, float _21, float _31, float _41, float _12, float _22, float _32, float _42, float _13, float _23, float _33, float _43, float _14, float _24, float _34, float _44) : size(16)
{
	_m11 = _11;
	_m21 = _21;
	_m31 = _31;
	_m41 = _41;
	_m12 = _12;
	_m22 = _22;
	_m32 = _32;
	_m42 = _42;
	_m13 = _13;
	_m23 = _23;
	_m33 = _33;
	_m43 = _43;
	_m14 = _14;
	_m24 = _24;
	_m34 = _34;
	_m44 = _44;
}

Matrix4x4::Matrix4x4(float allValues) : size(16)
{
	for(int i=0; i<size; i++)
	{
		elements[i] = allValues;
	}
}

Matrix4x4::Matrix4x4() : size(16)
{
	memcpy(elements, Matrix4x4::ZERO.elements, sizeof(float)*size);
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::setPerspective(float fieldOfView, float screenAspect, float screenNear, float screenDepth)
{
	elements[0]  = 1.0f / (screenAspect * tan(fieldOfView * 0.5f));
	elements[1]  = 0.0f;
	elements[2]  = 0.0f;
	elements[3]  = 0.0f;

	elements[4]  = 0.0f;
	elements[5]  = 1.0f / tan(fieldOfView * 0.5f);
	elements[6]  = 0.0f;
	elements[7]  = 0.0f;

	elements[8]  = 0.0f;
	elements[9]  = 0.0f;
	elements[10] = screenDepth / (screenDepth - screenNear);
	elements[11] = 1.0f;

	elements[12] = 0.0f;
	elements[13] = 0.0f;
	elements[14] = (-screenNear * screenDepth) / (screenDepth - screenNear);
	elements[15] = 0.0f;
}

void Matrix4x4::setOrthoProj( float screenAspect, float screenNear, float screenDepth)
{
	float right = 1.0f;
	float left = -1.0f;
	float top = screenAspect;
	float bottom = -1*screenAspect;

	elements[0] = 2 / (right - left);
	elements[1] = 0;
	elements[2] = 0;
	elements[3] = 0;
	elements[4] = 0;
	elements[5] = 2 / (top - bottom);
	elements[6] = 0;
	elements[7] = 0;
	elements[8] = 0;
	elements[9] = 0;
	elements[10] = -1 / (screenDepth - screenNear);
	elements[11] = 0;
	elements[12] = -(right + left) / (right - left);
	elements[13] = -(top + bottom) / (top - bottom);
	elements[14] = -screenNear / (screenDepth - screenNear);
	elements[15] = 1;
}

int Matrix4x4::getSize()
{
	return 16;
}

float* Matrix4x4::getElements()
{
	return elements;
}

void Matrix4x4::operator=(const Matrix4x4 &mat)
{
	for(int i=0; i<size; i++)
	{
		elements[i] = mat.elements[i];
	}
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &mat) const
{
	Matrix4x4 result;
	for(int i=0; i<size; i++)
	{
		result.elements[i] = elements[i] + mat.elements[i];
	}
	return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &mat) const
{
	Matrix4x4 result;
	for(int i=0; i<size; i++)
	{
		result.elements[i] = elements[i] - mat.elements[i];
	}
	return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &mat) const
{
	Matrix4x4 temp = Matrix4x4();

	for(int row=0; row<4; row++)
	{
		for(int col=0; col<4; col++)
		{
			for(int add=0; add<4; add++)
			{
				temp.getElements()[row*4 + col] += elements[row + add*4] * mat.elements[4*col + add];
			}
		}
	}

	return temp;
}

Matrix4x4 Matrix4x4::operator*(const float &num) const
{
	Matrix4x4 result;
	for(int i=0; i<size; i++)
	{
		result.elements[i] = elements[i] * num;
	}
	return result;
}

Matrix4x4 Matrix4x4::getInverse()
{
	float det;
	Matrix4x4 inv;
	int i;

	inv.getElements()[0] = elements[5]  * elements[10] * elements[15] - 
			 elements[5]  * elements[11] * elements[14] - 
			 elements[9]  * elements[6]  * elements[15] + 
			 elements[9]  * elements[7]  * elements[14] +
			 elements[13] * elements[6]  * elements[11] - 
			 elements[13] * elements[7]  * elements[10];

	inv.getElements()[4] = -elements[4]  * elements[10] * elements[15] + 
			  elements[4]  * elements[11] * elements[14] + 
			  elements[8]  * elements[6]  * elements[15] - 
			  elements[8]  * elements[7]  * elements[14] - 
			  elements[12] * elements[6]  * elements[11] + 
			  elements[12] * elements[7]  * elements[10];

	inv.getElements()[8] = elements[4]  * elements[9] * elements[15] - 
			 elements[4]  * elements[11] * elements[13] - 
			 elements[8]  * elements[5] * elements[15] + 
			 elements[8]  * elements[7] * elements[13] + 
			 elements[12] * elements[5] * elements[11] - 
			 elements[12] * elements[7] * elements[9];

	inv.getElements()[12] = -elements[4]  * elements[9] * elements[14] + 
			   elements[4]  * elements[10] * elements[13] +
			   elements[8]  * elements[5] * elements[14] - 
			   elements[8]  * elements[6] * elements[13] - 
			   elements[12] * elements[5] * elements[10] + 
			   elements[12] * elements[6] * elements[9];

	inv.getElements()[1] = -elements[1]  * elements[10] * elements[15] + 
			  elements[1]  * elements[11] * elements[14] + 
			  elements[9]  * elements[2] * elements[15] - 
			  elements[9]  * elements[3] * elements[14] - 
			  elements[13] * elements[2] * elements[11] + 
			  elements[13] * elements[3] * elements[10];

	inv.getElements()[5] = elements[0]  * elements[10] * elements[15] - 
			 elements[0]  * elements[11] * elements[14] - 
			 elements[8]  * elements[2] * elements[15] + 
			 elements[8]  * elements[3] * elements[14] + 
			 elements[12] * elements[2] * elements[11] - 
			 elements[12] * elements[3] * elements[10];

	inv.getElements()[9] = -elements[0]  * elements[9] * elements[15] + 
			  elements[0]  * elements[11] * elements[13] + 
			  elements[8]  * elements[1] * elements[15] - 
			  elements[8]  * elements[3] * elements[13] - 
			  elements[12] * elements[1] * elements[11] + 
			  elements[12] * elements[3] * elements[9];

	inv.getElements()[13] = elements[0]  * elements[9] * elements[14] - 
			  elements[0]  * elements[10] * elements[13] - 
			  elements[8]  * elements[1] * elements[14] + 
			  elements[8]  * elements[2] * elements[13] + 
			  elements[12] * elements[1] * elements[10] - 
			  elements[12] * elements[2] * elements[9];

	inv.getElements()[2] = elements[1]  * elements[6] * elements[15] - 
			 elements[1]  * elements[7] * elements[14] - 
			 elements[5]  * elements[2] * elements[15] + 
			 elements[5]  * elements[3] * elements[14] + 
			 elements[13] * elements[2] * elements[7] - 
			 elements[13] * elements[3] * elements[6];

	inv.getElements()[6] = -elements[0]  * elements[6] * elements[15] + 
			  elements[0]  * elements[7] * elements[14] + 
			  elements[4]  * elements[2] * elements[15] - 
			  elements[4]  * elements[3] * elements[14] - 
			  elements[12] * elements[2] * elements[7] + 
			  elements[12] * elements[3] * elements[6];

	inv.getElements()[10] = elements[0]  * elements[5] * elements[15] - 
			  elements[0]  * elements[7] * elements[13] - 
			  elements[4]  * elements[1] * elements[15] + 
			  elements[4]  * elements[3] * elements[13] + 
			  elements[12] * elements[1] * elements[7] - 
			  elements[12] * elements[3] * elements[5];

	inv.getElements()[14] = -elements[0]  * elements[5] * elements[14] + 
			   elements[0]  * elements[6] * elements[13] + 
			   elements[4]  * elements[1] * elements[14] - 
			   elements[4]  * elements[2] * elements[13] - 
			   elements[12] * elements[1] * elements[6] + 
			   elements[12] * elements[2] * elements[5];

	inv.getElements()[3] = -elements[1] * elements[6] * elements[11] + 
			  elements[1] * elements[7] * elements[10] + 
			  elements[5] * elements[2] * elements[11] - 
			  elements[5] * elements[3] * elements[10] - 
			  elements[9] * elements[2] * elements[7] + 
			  elements[9] * elements[3] * elements[6];

	inv.getElements()[7] = elements[0] * elements[6] * elements[11] - 
			 elements[0] * elements[7] * elements[10] - 
			 elements[4] * elements[2] * elements[11] + 
			 elements[4] * elements[3] * elements[10] + 
			 elements[8] * elements[2] * elements[7] - 
			 elements[8] * elements[3] * elements[6];

	inv.getElements()[11] = -elements[0] * elements[5] * elements[11] + 
			   elements[0] * elements[7] * elements[9] + 
			   elements[4] * elements[1] * elements[11] - 
			   elements[4] * elements[3] * elements[9] - 
			   elements[8] * elements[1] * elements[7] + 
			   elements[8] * elements[3] * elements[5];

	inv.getElements()[15] = elements[0] * elements[5] * elements[10] - 
			  elements[0] * elements[6] * elements[9] - 
			  elements[4] * elements[1] * elements[10] + 
			  elements[4] * elements[2] * elements[9] + 
			  elements[8] * elements[1] * elements[6] - 
			  elements[8] * elements[2] * elements[5];

	det = elements[0] * inv.getElements()[0] + elements[1] * inv.getElements()[4] + elements[2] * inv.getElements()[8] + elements[3] * inv.getElements()[12];

	if (det == 0)
		return NULL;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		inv.getElements()[i] *= det;

	return inv;
}

Matrix4x4 Matrix4x4::getTranspose()
{
	Matrix4x4 transpose;
	for(int col=0; col<4; col++)
	{
		for(int row=0; row<4; row++)
		{
			transpose.elements[col*4 + row] = elements[row*4 + col];
		}
	}

	return transpose;
}

Matrix4x4 Matrix4x4::getCross(Matrix4x4 &mat)
{
	return Matrix4x4();
}

Matrix4x4 Matrix4x4::getMinor(Matrix4x4 &mat)
{
	return Matrix4x4();
}

void Matrix4x4::transpose()
{
	//Matrix4x4 transpose = getTranspose();
	//*this = transpose;
}

const Matrix4x4 Matrix4x4::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

const Matrix4x4 Matrix4x4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

Matrix3x3 mat3(Matrix4x4 &matrix)
{
	Matrix3x3 newMat(matrix._m11, matrix._m21, matrix._m31, matrix._m12, matrix._m22, matrix._m32, matrix._m13, matrix._m23, matrix._m33);
	return newMat;
}