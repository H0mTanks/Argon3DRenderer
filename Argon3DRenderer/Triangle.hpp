#pragma once
#include "Vector.hpp"
#include "Color.hpp"
#include "Texture.hpp"

class Face
{
public:
	int a = 0, b = 0, c = 0;
	Texture2 a_uv, b_uv, c_uv;
	Color color = 0xFFFFFFFF;
public:
};

class Triangle2
{
public:
	std::array<Vector2, 3> points;
	std::array<Texture2, 3> tex_coords;
	Color color = 0xFFFFFFFF;
public:
	Triangle2();
	Triangle2(Vector2 a, Vector2 b, Vector2 c, Color color);
};


class Triangle3
{
public:
	std::array<Vector3, 3> points;
	std::array<Texture2, 3> tex_coords;
	mutable Vector3 face_normal = { 0,0,0 };
public:
	Triangle3();
	Triangle3(Vector3 a, Vector3 b, Vector3 c);
	void compute_face_normal() const;
	float normal_deviation(Vector3 const& v);
	bool backface(Vector3 const& camera_position) const;
};


class Triangle4
{
public:
	std::array<Vector4, 3> points;
	std::array<Texture2, 3> tex_coords;
	mutable Vector3 face_normal = { 0,0,0 };
	Color color = 0xFFFFFFFF;
	upng_t* texture;
public:
	Triangle3 to_triangle3();
	Triangle2 to_triangle2();
};