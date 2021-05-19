#include "PrecompiledHeaders.hpp"
#include "Mesh.hpp"

#define MAX_MESHES 10

static Mesh meshes[MAX_MESHES];
static unsigned int mesh_count = 0;

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

	/*Face vertex_index;
	Face texture_index;*/
	//Face normal_index;

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
						Texture2 texture;
						sscanf_s(line, "vt %f %f", &texture.u, &texture.v);
						textures.push_back(texture);
					} break;
				}
			} break;

			case 'f': { // Face information
				int vertex_indices[3];
				int texture_indices[3];
				int normal_indices[3];
				int match = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
					&vertex_indices[0], &texture_indices[0], &normal_indices[0],
					&vertex_indices[1], &texture_indices[1], &normal_indices[1],
					&vertex_indices[2], &texture_indices[2], &normal_indices[2]
				);
				if (match == 9) {
					Face face = { vertex_indices[0] - 1, vertex_indices[1] - 1, vertex_indices[2] - 1,
						textures[texture_indices[0] - 1], textures[texture_indices[1] - 1], textures[texture_indices[2] - 1]
					};

					faces.push_back(face);
					/*texture_indices.push_back(texture_index);
					normal_indices.push_back(normal_index);*/
				}
				/*faces.push_back(vertex_index);*/
			} break;
		}
	}

	return true;
}

bool Mesh::load_png_mesh_data(const char* filepath)
{
	upng_t* png_image = upng_new_from_file(filepath);
	if (png_image) {
		upng_decode(png_image);
		if (upng_get_error(png_image) == UPNG_EOK) {
			this->texture = png_image;
		}
	}
	return false;
}

Mesh::~Mesh()
{
	//upng_free(texture);
}

void load_mesh(const char* obj_filename, const char* png_filename, Vector3 scale, Vector3 translation, Vector3 rotation)
{
	meshes[mesh_count].load_obj_mesh_data(obj_filename);
	meshes[mesh_count].load_png_mesh_data(png_filename);

	meshes[mesh_count].scale = scale;
	meshes[mesh_count].translation = translation;
	meshes[mesh_count].rotation = rotation;

	mesh_count++;
}

int get_number_of_meshes()
{
	return mesh_count;
}

Mesh& get_mesh(int index)
{
	return meshes[index];
}
