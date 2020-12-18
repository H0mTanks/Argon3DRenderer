#include "Triangle.hpp"

bool Triangle3D::backface(Vector3 const& camera_position) const
{
	Vector3 const& a = (*this).points[0];
	Vector3 const& b = (*this).points[1];
	Vector3 const& c = (*this).points[2];

	Vector3 ab = b.sub(a);
	Vector3 ac = c.sub(a);

	Vector3 normal = ab.cross(ac); //v1 x v2

	Vector3 camera_ray = camera_position.sub(a);

	return normal.dot(camera_ray) < 0;
}
