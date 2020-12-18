#pragma once
#include <array>
#include "Vector.hpp"


class Face
{
public:
	int a, b, c;
};

class Triangle2D
{
public:
	std::array<Vector2, 3> points;
};

class Triangle3D
{
public:
	std::array<Vector3, 3> points;
public:
	bool backface(Vector3 const& camera_position) const;
};