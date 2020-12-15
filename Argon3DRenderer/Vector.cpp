#include <cmath>
#include "Vector.hpp"

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

Vector3::Vector3() {
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}


/// <summary>
/// Returns a Vector2 containing points for the orthographic projection of a Vector3 object
/// </summary>
/// <returns>Vector2 object</returns>
Vector2 Vector3::orthographic_project() const {
	float fov_factor = 128; //lowest for rec size 4 is 16
	Vector3 const& original_vector = *this;
	Vector2 projection_vector = { fov_factor * original_vector.x, fov_factor * original_vector.y };
	return projection_vector;
}

Vector2 Vector3::perspective_project() const
{
	float fov_factor = 640;
	Vector3 const& original_vector = *this;
	Vector2 projection_vector = { (fov_factor * original_vector.x) / original_vector.z,
		(fov_factor * original_vector.y) / original_vector.z };
	return projection_vector;
}

Vector3 Vector3::rotate_x(float angle) {
	Vector3& v = *this;
	Vector3 rotated_vector(
		v.x, 
		v.y * cos(angle) - v.z * sin(angle), 
		v.y * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}


Vector3 Vector3::rotate_y(float angle) {
	Vector3& v = *this;
	Vector3 rotated_vector(
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}


Vector3 Vector3::rotate_z(float angle) {
	Vector3& v = *this;
	Vector3 rotated_vector(
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	);
	return rotated_vector;
}

Vector3 Vector3::rotate_xyz(float angle_x, float angle_y, float angle_z) {
	Vector3 rotated_vector = *this;
	rotated_vector = rotated_vector.rotate_x(angle_x);
	rotated_vector = rotated_vector.rotate_y(angle_y);
	rotated_vector = rotated_vector.rotate_z(angle_z);

	return rotated_vector;
}


std::ostream& operator<<(std::ostream& stream, Vector2 const& v)
{
	stream << "x: " << v.x << " y: " << v.y;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Vector3 const& v)
{
	stream << "x: " << v.x << " y: " << v.y << " z: " << v.z;
	return stream;
}
