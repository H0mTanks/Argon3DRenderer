#pragma once
#include <iostream>

// Struct for holding a 2-Dimensional Vector
struct Vector2
{
	float x{ 0.0f };
	float y{ 0.0f };

public:
	Vector2();
	Vector2(float x, float y);

	float magnitude() const;
	Vector2 add(Vector2 const& v2) const;
	Vector2 sub(Vector2 const& v2) const;
	Vector2 mul(float factor) const;
	Vector2 div(float factor) const;
	float dot(Vector2 const& v2) const;
	Vector2 normalize() const;
};
std::ostream& operator<<(std::ostream& stream, Vector2 const& v);


// Struct for holding a 3-Dimensional Vector
struct Vector3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

public:
	Vector3();
	Vector3(float x, float y, float z);

	Vector2 orthographic_project() const;
	Vector2 perspective_project() const;

	Vector3 rotate_x(float angle) const;
	Vector3 rotate_y(float angle) const;
	Vector3 rotate_z(float angle) const;
	Vector3 rotate_xyz(float angle_x, float angle_y, float angle_z) const;

	float magnitude() const;
	Vector3 add(Vector3 const& v2) const;
	Vector3 sub(Vector3 const& v2) const;
	Vector3 mul(float factor) const;
	Vector3 div(float factor) const;
	float dot(Vector3 const& v2) const;
	Vector3 cross(Vector3 const& v2) const;
	Vector3 normalize() const;

};
std::ostream& operator<<(std::ostream& stream, Vector3 const& v);