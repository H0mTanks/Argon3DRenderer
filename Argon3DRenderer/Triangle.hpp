#pragma once
#include <array>
#include "Vector.hpp"


class Face
{
public:
	int a, b, c;
	uint32_t color;
};

class Triangle2
{
public:
	std::array<Vector2, 3> points;
	uint32_t color = 0x00000000;
	float depth = 0;
	bool operator<(Triangle2 const& t) const;
	bool operator>(Triangle2 const& t) const;
};


class Triangle3
{
public:
	std::array<Vector3, 3> points;
public:
	Triangle3();
	Triangle3(Vector3 a, Vector3 b, Vector3 c);
	bool backface(Vector3 const& camera_position) const;
};


class Triangle4
{
public:
	std::array<Vector4, 3> points;
public:
	Triangle3 to_triangle3();
};