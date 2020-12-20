#include <fstream>
#include <string>
#include <cstdio>
#include "Mesh.hpp"


Mesh::Mesh() {}


Mesh::Mesh(int num_vertices, int num_faces, Vector3& rotation_val)
{
	vertices.reserve(num_vertices);
	faces.reserve(num_faces);
}


bool Mesh::load_obj_mesh_data(const char* filepath)
{
	FILE* file;
	fopen_s(&file, filepath, "r");
	char line[1024];

	Face vertex_index;
	Face texture_index;
	Face normal_index;

	while (fgets(line, 1024, file)) {
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

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

Vector3 cube_vertices[N_CUBE_VERTICES] = {
    {-1, -1, -1 }, // 1
    {-1, 1, -1 }, // 2
    {1, 1, -1 }, // 3
    {1, -1, -1 }, // 4
    {1, 1, 1 }, // 5
    {1, -1, 1 }, // 6
    {-1, 1, 1 }, // 7
    {-1, -1, 1 }  // 8
};

Face cube_faces[N_CUBE_FACES] = {
    // front
    {1, 2, 3, 0xFFFFFFFF },
    {1, 3, 4, 0xFFFFFFFF },
    // right
    {4, 3, 5, 0xFFFFFFFF },
    {4, 5, 6, 0xFFFFFFFF },
    // back
    {6, 5, 7, 0xFFFFFFFF },
    {6, 7, 8, 0xFFFFFFFF },
    // left
    {8, 7, 2, 0xFFFFFFFF },
    {8, 2, 1, 0xFFFFFFFF },
    // top
    {2, 7, 5, 0xFFFFFFFF },
    {2, 5, 3, 0xFFFFFFFF },
    // bottom
    {6, 8, 1, 0xFFFFFFFF },
    {6, 1, 4, 0xFFFFFFFF }
};

void Mesh::load_cube_mesh_data()
{
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		Vector3 cube_vertex = cube_vertices[i];
		vertices.push_back(cube_vertex);
	}
	for (int i = 0; i < N_CUBE_FACES; i++) {
		Face cube_face = cube_faces[i];
		faces.push_back(cube_face);
	}
}