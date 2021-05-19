#pragma once
#include "Vector.hpp"
#include "Triangle.hpp"


class Mesh
{
public:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals; //added
	std::vector<Texture2> textures; //added
	std::vector<Face> faces;
	upng_t* texture;
	//std::vector<Face> normal_indices;
	//std::vector<Face> texture_indices;
	Vector3 rotation = { 0, 0, 0 };
	Vector3 scale = { 1, 1, 1 };
	Vector3 translation = { 0, 0, 0 };

public:
	Mesh();
	Mesh(int num_vertices, int num_faces, Vector3& rotation);
	bool load_obj_mesh_data(const char* filepath);
	bool load_png_mesh_data(const char* filepath);
	~Mesh();
};

void load_mesh(const char* obj_filename, const char* png_filename, Vector3 scale, Vector3 translation, Vector3 rotation);
int get_number_of_meshes();
Mesh& get_mesh(int index);