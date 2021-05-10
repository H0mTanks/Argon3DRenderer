#include "PrecompiledHeaders.hpp"
#include "Clipping.hpp"

#define NUM_PLANES 6
std::array<Plane, NUM_PLANES> frustum_planes;

void init_frustum_planes(float fov_x, float fov_y, float z_near, float z_far)
{
	float cos_half_fov_x = cos(fov_x / 2);
	float sin_half_fov_x = sin(fov_x / 2);
	float cos_half_fov_y = cos(fov_y / 2);
	float sin_half_fov_y = sin(fov_y / 2);



	Vector3 origin(0, 0, 0);

	frustum_planes[LEFT_FRUSTUM_PLANE].point = origin;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov_x;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

	frustum_planes[RIGHT_FRUSTUM_PLANE].point = origin;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov_x;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

	frustum_planes[TOP_FRUSTUM_PLANE].point = origin;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov_y;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

	frustum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov_y;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

	frustum_planes[NEAR_FRUSTUM_PLANE].point = { 0, 0, z_near };
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;


	frustum_planes[FAR_FRUSTUM_PLANE].point = { 0, 0, z_far };
	frustum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

Polygon create_polygon_from_triangle(Triangle3 const& triangle, Texture2 const& t0, Texture2 const& t1, Texture2 const& t2)
{
	Polygon polygon;
	polygon.vertices[0] = triangle.points[0];
	polygon.vertices[1] = triangle.points[1];
	polygon.vertices[2] = triangle.points[2];
	polygon.tex_coords[0] = t0;
	polygon.tex_coords[1] = t1;
	polygon.tex_coords[2] = t2;

	polygon.num_vertices = 3;

	return polygon;
}

void Polygon::clip_polygon()
{
	clip_polygon_against_plane(LEFT_FRUSTUM_PLANE);
	clip_polygon_against_plane(RIGHT_FRUSTUM_PLANE);
	clip_polygon_against_plane(TOP_FRUSTUM_PLANE);
	clip_polygon_against_plane(BOTTOM_FRUSTUM_PLANE);
	clip_polygon_against_plane(NEAR_FRUSTUM_PLANE);
	clip_polygon_against_plane(FAR_FRUSTUM_PLANE);

}


float float_lerp(float a, float b, float t)
{
	return a + (t * (b - a));
}


void Polygon::clip_polygon_against_plane(int plane_type)
{
	Polygon& polygon = *this;
	if (polygon.num_vertices == 0) return;

	Vector3 plane_point = frustum_planes[plane_type].point;
	Vector3 plane_normal = frustum_planes[plane_type].normal;

	std::array<Vector3, MAX_NUM_POLY_VERTICES> inside_vertices;
	std::array<Texture2, MAX_NUM_POLY_VERTICES> inside_tex_coords;
	size_t num_inside_vertices = 0;

	Vector3* current_vertex = &polygon.vertices[0];
	Vector3* prev_vertex = &polygon.vertices[polygon.num_vertices - 1];

	Texture2* current_tex_coord = &polygon.tex_coords[0];
	Texture2* prev_tex_coord = &polygon.tex_coords[polygon.num_vertices - 1];

	float current_dot = (current_vertex->sub(plane_point)).dot(plane_normal);
	float prev_dot = (prev_vertex->sub(plane_point)).dot(plane_normal);

	while (current_vertex != &polygon.vertices[polygon.num_vertices]) {
		if (current_dot * prev_dot < 0) {
			float t = prev_dot / (prev_dot - current_dot);

			// I = Q1 + t(Q2 - Q1);
			Vector3 intersection_point;
			intersection_point.x = float_lerp(prev_vertex->x, current_vertex->x, t);
			intersection_point.y = float_lerp(prev_vertex->y, current_vertex->y, t);
			intersection_point.z = float_lerp(prev_vertex->z, current_vertex->z, t);

			Texture2 interpolated_tex_coord;
			interpolated_tex_coord.u = float_lerp(prev_tex_coord->u, current_tex_coord->u, t);
			interpolated_tex_coord.v = float_lerp(prev_tex_coord->v, current_tex_coord->v, t);

			inside_vertices[num_inside_vertices] = intersection_point;
			inside_tex_coords[num_inside_vertices] = interpolated_tex_coord;
			num_inside_vertices++;
		}

		if (current_dot > 0) {
			inside_vertices[num_inside_vertices] = *current_vertex;
			inside_tex_coords[num_inside_vertices] = *current_tex_coord;
			num_inside_vertices++;
		}

		prev_vertex = current_vertex;
		prev_dot = current_dot;
		prev_tex_coord = current_tex_coord;
		current_vertex++;
		current_tex_coord++;
		current_dot = (current_vertex->sub(plane_point)).dot(plane_normal);
	}

	for (int i = 0; i < num_inside_vertices; i++) {
		polygon.vertices[i] = inside_vertices[i];
		polygon.tex_coords[i] = inside_tex_coords[i];
	}
	polygon.num_vertices = num_inside_vertices;
}

void Polygon::triangles_from_polygon(std::array<Triangle3, MAX_NUM_POLY_TRIANGLES>& triangles, int& num_triangles)
{
	for (size_t i = 0; i < this->num_vertices - 2; i++) {
		triangles[i].points[0] = this->vertices[i];
		triangles[i].points[1] = this->vertices[i + 1];
		triangles[i].points[2] = this->vertices[i + 2];
		triangles[i].tex_coords[0] = this->tex_coords[i];
		triangles[i].tex_coords[1] = this->tex_coords[i + 1];
		triangles[i].tex_coords[2] = this->tex_coords[i + 2];

	}

	num_triangles = this->num_vertices - 2;
}
