#pragma once

#include <vector>
#include "Vector.hpp"
#include "Triangle.hpp"


class Mesh
{
public:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals; //added
	std::vector<Vector2> textures; //added
	std::vector<Face> faces;
	std::vector<Face> normal_indices;
	std::vector<Face> texture_indices;
	Vector3 rotation = { 0, 0, 0 };
	Vector3 scale = { 1, 1, 1 };
	Vector3 translation = { 0, 0, 0 };

public:
	Mesh();
	Mesh(int num_vertices, int num_faces, Vector3& rotation);
	bool load_obj_mesh_data(const char* filepath);
    void load_cube_mesh_data();
};