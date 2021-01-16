// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "PrecompiledHeaders.hpp"
#include "App.hpp"
#include "Vector.hpp"
#include "Mesh.hpp"
#include "Matrix.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "Texture.hpp"
#include "Profiler.hpp"

Vector3 camera_position = Vector3(0, 0, 0);
std::vector<Triangle2> triangles_to_render;

Mesh mesh;
Light global_light;

Matrix4 projection_matrix;

int App::WINDOW_WIDTH = 800;
int App::WINDOW_HEIGHT = 600;

SDL_Renderer* App::renderer = nullptr;
SDL_Texture* App::display_buffer_texture = nullptr;
Color* App::display_buffer = nullptr;



// Initializes SDL, Window, Renderer and DisplayBuffer
App::App()
{
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Error Initializing SDL" << '\n';
		return;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	WINDOW_WIDTH = display_mode.w;
	WINDOW_HEIGHT = display_mode.h;

	window = SDL_CreateWindow(
		"Argon",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS);

	if (!window) {
		std::cerr << "Error creating SDL Window" << '\n';
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0/*SDL_RENDERER_PRESENTVSYNC*/);
	if (!renderer) {
		std::cerr << "Error creating SDL_Renderer" << '\n';
		return;
	}

	setup_display();

	is_running = true;
	return;
}



// Processes User Input and Events
void App::process_input()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: {
			is_running = false;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit();
			}
			if (event.key.keysym.sym == SDLK_1) {
				render_type = Draw::Render_type::RENDER_WIREFRAME_VERTEX;
			}
			if (event.key.keysym.sym == SDLK_2) {
				render_type = Draw::Render_type::RENDER_WIREFRAME;
			}
			if (event.key.keysym.sym == SDLK_3) {
				render_type = Draw::Render_type::RENDER_FILL_TRIANGLE;
			}
			if (event.key.keysym.sym == SDLK_4) {
				render_type = Draw::Render_type::RENDER_FILL_TRIANGLE_WIREFRAME;
			}
			if (event.key.keysym.sym == SDLK_5) {
				render_type = Draw::Render_type::RENDER_TEXTURED;
			}
			if (event.key.keysym.sym == SDLK_6) {
				render_type = Draw::Render_type::RENDER_TEXTURED_WIRE;
			}
			if (event.key.keysym.sym == SDLK_c) {
				cull_type = Draw::Cull_type::CULL_BACKFACE;
			}
			if (event.key.keysym.sym == SDLK_d) {
				cull_type = Draw::Cull_type::CULL_NONE;
			}
			break;
		}
	}
}


// Updates each frame
void App::update()
{
	PROFILE_FUNCTION();

	mesh.rotation.x += 0.01f;
	mesh.rotation.y += 0.01f;
	mesh.rotation.z += 0.01f;

	mesh.scale.x = 0.5;
	mesh.scale.y = 0.5;
	mesh.scale.z = 0.5;

	//mesh.translation.x += 0.01f;
	mesh.translation.z = 5.0f;
	//mesh.translation.y = -1.0f;

	Matrix4 scale_matrix = Matrix4::make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	Matrix4 translation_matrix = Matrix4::make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	Matrix4 rotation_matrix_x = Matrix4::make_rotation_x(mesh.rotation.x);
	Matrix4 rotation_matrix_y = Matrix4::make_rotation_y(mesh.rotation.y);
	Matrix4 rotation_matrix_z = Matrix4::make_rotation_z(mesh.rotation.z);


	Triangle3 transformed_triangle;
	Triangle2 projected_triangle;

	for (int i = 0; i < mesh.faces.size(); i++) {
		Face& mesh_face = mesh.faces[i];

		std::array<Vector3, 3> face_vertices;
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		for (int j = 0; j < 3; j++) {
			Vector4 transformed_vertex = face_vertices[j].to_vec4();
			Matrix4 world_matrix = Matrix4::make_world(scale_matrix, rotation_matrix_x, 
				rotation_matrix_y, rotation_matrix_z, translation_matrix);
			transformed_vertex = world_matrix.mul_vector(transformed_vertex);

			transformed_triangle.points[j] = transformed_vertex.to_vec3();
		}
		transformed_triangle.compute_face_normal();


		if (cull_type == Draw::Cull_type::CULL_BACKFACE) {
			if (transformed_triangle.backface(camera_position)) {
				continue;
			}
		}


		float avg_depth = (transformed_triangle.points[0].z + transformed_triangle.points[1].z + transformed_triangle.points[2].z) / 3.0f;

		for (int j = 0; j < 3; j++) {
			Vector4 projected_point = projection_matrix.mul_project(transformed_triangle.points[j].to_vec4());

			projected_point.x *= WINDOW_WIDTH * 0.5f;
			projected_point.y *= WINDOW_HEIGHT * 0.5f;

			projected_point.y *= -1;

			projected_point.x += WINDOW_WIDTH * 0.5f;
			projected_point.y += WINDOW_HEIGHT * 0.5f;


			projected_triangle.points[j] = projected_point.to_vec3().orthographic_project();
		}

		float light_factor = -transformed_triangle.normal_deviation(global_light.direction);
		projected_triangle.tex_coords = { mesh_face.a_uv, mesh_face.b_uv, mesh_face.c_uv };
		projected_triangle.color = global_light.intensity(mesh_face.color, light_factor);
		projected_triangle.depth = avg_depth;
		triangles_to_render.push_back(projected_triangle);

		//std::stable_sort(triangles_to_render.begin(), triangles_to_render.end(), std::greater<Triangle2>());
	}

}


// Renders the current display buffer
void App::render()
{
	PROFILE_FUNCTION();

	Draw::grid();

	int trianglesize = triangles_to_render.size();
	for (int i = 0; i < trianglesize; i++) {
		Triangle2 triangle = triangles_to_render[i];
		if (render_type == Draw::Render_type::RENDER_FILL_TRIANGLE || render_type == Draw::Render_type::RENDER_FILL_TRIANGLE_WIREFRAME) {
			Draw::fill_triangle(triangle, triangle.color);
		}

		if (render_type == Draw::Render_type::RENDER_FILL_TRIANGLE_WIREFRAME || render_type == Draw::Render_type::RENDER_TEXTURED_WIRE
			|| render_type == Draw::Render_type::RENDER_WIREFRAME_VERTEX || render_type == Draw::Render_type::RENDER_WIREFRAME) {
			Draw::triangle(triangle, 0xDDDDDD00);
		}

		if (render_type == Draw::Render_type::RENDER_TEXTURED || render_type == Draw::Render_type::RENDER_TEXTURED_WIRE) {
			Draw::textured_triangle(triangle, mesh_texture);
		}

		if (render_type == Draw::Render_type::RENDER_WIREFRAME_VERTEX) {
			Draw::rectangle(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFFFF00);
			Draw::rectangle(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFFFF00);
			Draw::rectangle(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFFFF00);
		}
	}

	render_display_buffer();

	triangles_to_render.clear();
	clear_display_buffer();

	SDL_RenderPresent(renderer);
}


//	Sets up the display buffer and should be called during Initialisation
void App::setup_display()
{
	display_buffer = new Color[WINDOW_HEIGHT * WINDOW_WIDTH];
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(Color));

	if (!display_buffer) {
		std::cout << "Could not allocate memory for buffer";
	}

	display_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

	/*if (!mesh.load_obj_mesh_data("./assets/duck2.obj")) {
		quit();
		return;
	}*/

	constexpr float fov = 60.0f * static_cast<float>(M_PI) / 180.0f;
	float aspect = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
	constexpr float znear = 0.1f;
	constexpr float zfar = 100.0f;
	projection_matrix = Matrix4::make_perspective(fov, aspect, znear, zfar);

	mesh_texture = (Color*)(REDBRICK_TEXTURE);
	texture_height = 64;
	texture_width = 64;

	mesh.load_cube_mesh_data();

	std::cout << "done" << '\n';

}


void App::clear_display_buffer()
{
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(Color));
}


void App::quit()
{
	is_running = false;
	return;
}


void App::destroy()
{
	delete[] display_buffer;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void App::render_display_buffer()
{
	SDL_UpdateTexture(display_buffer_texture, NULL, display_buffer, WINDOW_WIDTH * sizeof(Color));
	SDL_RenderCopy(renderer, display_buffer_texture, NULL, NULL);
}