#pragma once
#include "Vector.hpp"
#include "Color.hpp"

class Light
{
public:
	Vector3 direction = {0, 0, 1};
	
public:
	Light();
	Light(float x, float y, float z);
	Light(Vector3 const& v);
	Color intensity(Color color, float factor);
};


