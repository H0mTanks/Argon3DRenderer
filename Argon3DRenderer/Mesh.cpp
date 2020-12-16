#include <fstream>
#include <string>
#include <cstdio>
#include "Mesh.hpp"


Mesh::Mesh() {}


Mesh::Mesh(int num_vertices, int num_faces, Vector3& rotation_val)	{
	vertices.reserve(num_vertices);
	faces.reserve(num_faces);
}


#define N_CUBE_VERTICES 8
std::vector<Vector3> cube_vertices = {
   {-1 , -1, -1},
   {-1 ,  1, -1},
   { 1 ,  1, -1},
   { 1 , -1, -1},
   { 1 ,  1,  1},
   { 1 , -1,  1},
   {-1 ,  1,  1},
   {-1 , -1,  1},
};

#define N_CUBE_FACES (6 * 2)
std::vector<Face> cube_faces = {
	{1, 2, 3},
	{1, 3, 4},
	{4, 3, 5},
	{4, 5, 6},
	{6, 5, 7},
	{6, 7, 8},
	{8, 7, 2},
	{8, 2, 1},
	{2, 7, 5},
	{2, 5, 3},
	{6, 8, 1},
	{6, 1, 4},
};


void Mesh::load_cube_mesh_data() {
	for (std::vector<Vector3>::iterator::value_type& vertex : cube_vertices) {
		vertices.push_back(vertex);
	}

	for (std::vector<Face>::iterator::value_type& face : cube_faces) {
		faces.push_back(face);
	}
}


bool Mesh::load_obj_mesh_data(const char* filepath) {
    FILE* file;
    fopen_s(&file, filepath, "r");
    char line[2048];

    Face vertex_index;
    Face texture_index;
    Face normal_index;

    while (fgets(line, 2048, file)) {
        switch (line[0]) {
            case 'v': { // Vertex information
                switch (line[1]) {
                    case ' ': {
                        Vector3 vertex;
                        sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                        vertices.push_back(vertex);
                    } break;
                    case 'n': {
                        Vector3 normal;
                        sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                        normals.push_back(normal);
                    } break;
                    case 't': {
                        Vector2 texture;
                        sscanf_s(line, "vt %f %f", &texture.x, &texture.y);
                        textures.push_back(texture);
                    } break;
                }
            } break;

            case 'f': { // Face information
                int match = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertex_index.a, &texture_index.a, &normal_index.a,
                    &vertex_index.b, &texture_index.b, &normal_index.b,
                    &vertex_index.c, &texture_index.c, &normal_index.c
                );
                if (match == 9) {
                    texture_indices.push_back(texture_index);
                    normal_indices.push_back(normal_index);
                }
                else {
                    match = sscanf_s(line, "f %d/%d %d/%d %d/%d",
                        &vertex_index.a, &texture_index.a,
                        &vertex_index.b, &texture_index.b,
                        &vertex_index.c, &texture_index.c
                    );
                    if (match == 6) {
                        texture_indices.push_back(texture_index);
                    }
                    else {
                        sscanf_s(line, "f %d %d %d",
                            &vertex_index.a,
                            &vertex_index.b,
                            &vertex_index.c
                        );
                    }
                }
                faces.push_back(vertex_index);
            } break;
        }
    }

    return true;
}