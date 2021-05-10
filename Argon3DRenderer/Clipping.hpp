#pragma once
#include "Vector.hpp"
#include "Triangle.hpp"

#define MAX_NUM_POLY_VERTICES 101
#define MAX_NUM_POLY_TRIANGLES 1000

enum
{
	LEFT_FRUSTUM_PLANE,
	RIGHT_FRUSTUM_PLANE,
	TOP_FRUSTUM_PLANE,
	BOTTOM_FRUSTUM_PLANE,
	NEAR_FRUSTUM_PLANE,
	FAR_FRUSTUM_PLANE,
};

struct Plane
{
	Vector3 point;
	Vector3 normal;
};

void init_frustum_planes(float fov_x, float fov_y, float z_near, float z_far);

struct Polygon
{
	std::array<Vector3, MAX_NUM_POLY_VERTICES> vertices;
	std::array<Texture2, MAX_NUM_POLY_VERTICES> tex_coords;
	size_t num_vertices;

	void clip_polygon();
	void clip_polygon_against_plane(int plane_type);
	void triangles_from_polygon(std::array<Triangle3, MAX_NUM_POLY_TRIANGLES>& triangles, int& num_triangles);
};

Polygon create_polygon_from_triangle(Triangle3 const& triangle, Texture2 const& t0, Texture2 const& t1, Texture2 const& t2);