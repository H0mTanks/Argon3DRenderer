#pragma once
#include <iostream>

// Struct for holding a 2-Dimensional Vector
struct Vector2 {
	float x{ 0.0f };
	float y{ 0.0f };

	Vector2();
	Vector2(float x, float y);
};
std::ostream& operator<<(std::ostream& stream, Vector2 const& v);


// Struct for holding a 3-Dimensional Vector
struct Vector3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3();
	Vector3(float x, float y, float z);
	Vector2 orthographic_project() const;
	Vector2 perspective_project() const;
	Vector3 rotate_x(float angle);
	Vector3 rotate_y(float angle);
	Vector3 rotate_z(float angle);
	Vector3 rotate_xyz(float angle_x, float angle_y, float angle_z);
};
	std::ostream& operator<<(std::ostream& stream, Vector3 const& v);