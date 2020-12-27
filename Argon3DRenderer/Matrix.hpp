#pragma once
#include "Vector.hpp"

class Matrix4
{
public:
	std::array<std::array<float, 4>, 4> m;

public:
	Matrix4();
	Matrix4(float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44);
	static Matrix4 identity();
	static Matrix4 make_scale(float a11, float a22, float a33);
	static Matrix4 make_translation(float a14, float a24, float a34);
	static Matrix4 make_rotation_z(float angle);
	static Matrix4 make_rotation_y(float angle);
	static Matrix4 make_rotation_x(float angle);
	static Matrix4 make_world(Matrix4 const& scale, Matrix4 const& rotation_x, Matrix4 const& rotation_y,
		Matrix4 const& rotation_z, Matrix4 const& translation);
	static Matrix4 make_perspective(float fov, float aspect, float znear, float zfar);
	Vector4 mul_project(Vector4 const& v) const;

	Vector4 mul_vector(Vector4 const& v) const;
	Matrix4 mul_matrix(Matrix4 const& v) const;
};

