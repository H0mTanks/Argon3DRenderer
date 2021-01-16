#include "PrecompiledHeaders.hpp"
#include "Vector.hpp"

Vector2::Vector2()
{
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

float Vector2::magnitude() const
{
	Vector2 const& v = *this;
	return sqrt(v.x * v.x + v.y * v.y);
}

Vector2 Vector2::add(Vector2 const& v2) const
{
	Vector2 const& v1 = *this;
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 Vector2::sub(Vector2 const& v2) const
{
	Vector2 const& v1 = *this;
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 Vector2::mul(float factor) const
{
	Vector2 const& v = *this;
	return Vector2(v.x * factor, v.y * factor);
}

Vector2 Vector2::div(float factor) const
{
	Vector2 const& v = *this;
	return Vector2(v.x * factor, v.y * factor);
}

float Vector2::dot(Vector2 const& v2) const
{
	Vector2 const& v1 = *this;
	return (v1.x * v2.x + v1.y * v2.y);
}

Vector2 Vector2::normalize() const
{
	Vector2 const& v = *this;
	float magnitude = v.magnitude();
	return Vector2(v.x / magnitude, v.y / magnitude);
}


std::ostream& operator<<(std::ostream& stream, Vector2 const& v)
{
	stream << "x: " << v.x << " y: " << v.y;
	return stream;
}

Vector3::Vector3()
{
}


Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}


/// <summary>
/// Returns a Vector2 containing points for the orthographic projection of a Vector3 object
/// </summary>
/// <returns>Vector2 object</returns>
Vector2 Vector3::orthographic_project() const
{
	float fov_factor = 1; //duck 8
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

Vector3 Vector3::rotate_x(float angle) const
{
	Vector3 const& v = *this;
	Vector3 rotated_vector(
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	);
	return rotated_vector;
}


Vector3 Vector3::rotate_y(float angle) const
{
	Vector3 const& v = *this;
	Vector3 rotated_vector(
		v.x * cos(angle) + v.z * sin(angle),
		v.y,
		v.x * (-sin(angle)) + v.z * cos(angle)
	);
	return rotated_vector;
}


Vector3 Vector3::rotate_z(float angle) const
{
	Vector3 const& v = *this;
	Vector3 rotated_vector(
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	);
	return rotated_vector;
}

Vector3 Vector3::rotate_xyz(float angle_x, float angle_y, float angle_z) const
{
	Vector3 rotated_vector = *this;
	rotated_vector = rotated_vector.rotate_x(angle_x);
	rotated_vector = rotated_vector.rotate_y(angle_y);
	rotated_vector = rotated_vector.rotate_z(angle_z);

	return rotated_vector;
}



std::ostream& operator<<(std::ostream& stream, Vector3 const& v)
{
	stream << "x: " << v.x << " y: " << v.y << " z: " << v.z;
	return stream;
}

float Vector3::magnitude() const
{
	Vector3 const& v = *this;
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::add(Vector3 const& v2) const
{
	Vector3 const& v1 = *this;
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 Vector3::sub(Vector3 const& v2) const
{
	Vector3 const& v1 = *this;
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 Vector3::mul(float factor) const
{
	Vector3 const& v = *this;
	return Vector3(v.x * factor, v.y * factor, v.z * factor);
}

Vector3 Vector3::div(float factor) const
{
	Vector3 const& v = *this;
	return Vector3(v.x / factor, v.y / factor, v.z * factor);
}

float Vector3::dot(Vector3 const& v2) const
{
	Vector3 const& v1 = *this;
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Vector3 Vector3::cross(Vector3 const& v2) const
{
	Vector3 const& v1 = *this;
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vector3 Vector3::normalize() const
{
	Vector3 const& v = *this;
	float magnitude = v.magnitude();
	return Vector3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
}

Vector3 Vector3::barycentric_weights(Vector2_int const& a, Vector2_int const& b, Vector2_int const& c, Vector2_int const& p)
{
	Vector2_int ab = b.sub(a);
	Vector2_int bc = c.sub(b);
	Vector2_int ac = c.sub(a);
	Vector2_int ap = p.sub(a);
	Vector2_int bp = p.sub(b);

	float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);
	float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;
	float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;
	float gamma = 1 - alpha - beta;

	return Vector3(alpha, beta, gamma);
}

Vector4 Vector3::to_vec4()
{
	Vector3 const& v3 = *this;
	return Vector4(v3.x, v3.y, v3.z, 1.0f);
}

Vector2_int::Vector2_int()
{
}

Vector2_int::Vector2_int(int x, int y) : x(x), y(y)
{
}

Vector2_int Vector2_int::sub(Vector2_int const& v2) const
{
	Vector2_int const& v1 = *this;
	return Vector2_int(v1.x - v2.x, v1.y - v2.y);
}

Vector4::Vector4()
{
}

Vector4::Vector4(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{
}


Vector3 Vector4::to_vec3()
{
	Vector4 const& v4 = *this;
	return Vector3(v4.x, v4.y, v4.z);
}

Vector2 Vector4::to_vec2()
{
	Vector4 const& v4 = *this;
	return Vector2(v4.x, v4.y);
}

Vector4_int::Vector4_int()
{
}

Vector4_int::Vector4_int(int x, int y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vector2_int Vector4_int::to_vec2_int() const
{
	return Vector2_int(x, y);
}
