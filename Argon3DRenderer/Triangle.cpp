#include "PrecompiledHeaders.hpp"
#include "Triangle.hpp"

Triangle3::Triangle3()
{
}

Triangle3::Triangle3(Vector3 a, Vector3 b, Vector3 c)
{
	points = { a, b, c };
}

void Triangle3::compute_face_normal() const
{
	Triangle3 const& t = *this;
	Vector3 const& a = t.points[0];
	Vector3 const& b = t.points[1];
	Vector3 const& c = t.points[2];

	Vector3 ab = b.sub(a);
	Vector3 ac = c.sub(a);
	//ab = ab.normalize();
	//ac = ac.normalize();

	t.face_normal = ab.cross(ac); //v1 x v2
	t.face_normal = t.face_normal.normalize();
}

bool Triangle3::backface(Vector3 const& camera_position) const
{
	Triangle3 const& t = *this;
	Vector3 const& a = t.points[0];
	//Vector3 const& b = (*this).points[1];
	//Vector3 const& c = (*this).points[2];

	//Vector3 ab = b.sub(a);
	//Vector3 ac = c.sub(a);
	////ab = ab.normalize();
	////ac = ac.normalize();

	//Vector3 normal = ab.cross(ac); //v1 x v2
	//normal = normal.normalize();

	Vector3 camera_ray = camera_position.sub(a);

	return face_normal.dot(camera_ray) < 0;
}

bool Triangle2::operator<(Triangle2 const& t) const
{
	return depth < t.depth;
}

Triangle2::Triangle2()
{
}


Triangle2::Triangle2(Vector2 a, Vector2 b, Vector2 c, Color color, float depth)
{
	points = { a, b, c};
	this->color = color;
	this->depth = depth;
}


bool Triangle2::operator>(Triangle2 const& t) const
{
	return depth > t.depth;
}

Triangle3 Triangle4::to_triangle3()
{
	return Triangle3(points[0].to_vec3(), points[1].to_vec3(), points[2].to_vec3());
}

Triangle2 Triangle4::to_triangle2()
{
	return Triangle2(points[0].to_vec2(), points[1].to_vec2(), points[2].to_vec2(), color, depth);
}

float Triangle3::normal_deviation(Vector3 const& v)
{
	//Vector3 const& a = (*this).points[0];
	//Vector3 const& b = (*this).points[1];
	//Vector3 const& c = (*this).points[2];

	//Vector3 ab = b.sub(a);
	//Vector3 ac = c.sub(a);
	///*ab = ab.normalize();
	//ac = ac.normalize();*/

	//Vector3 normal = ab.cross(ac); //v1 x v2
	//normal = normal.normalize();
	return this->face_normal.dot(v);
}
