#pragma once
#include "VectorClass.h"

class Ray
{
public:
	Ray(void);
	~Ray(void);

	Vector3 origin;
	Vector3 direction;
};

