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
#include "upng.h"
#include "Camera.hpp"
#include "Clipping.hpp"

double old_frame_time = 0.0;
double delta_time = 0.0;

Camera camera;
std::vector<Triangle4> triangles_to_render;

Mesh mesh;
Light global_light;

Matrix4 world_matrix;
Matrix4 projection_matrix;
Matrix4 view_matrix;

int App::WINDOW_WIDTH = 800;
int App::WINDOW_HEIGHT = 600;

SDL_Renderer* App::renderer = nullptr;
SDL_Texture* App::display_buffer_texture = nullptr;
Color* App::display_buffer = nullptr;
float* App::z_buffer = nullptr;


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
			if (event.key.keysym.sym == SDLK_x) {
				cull_type = Draw::Cull_type::CULL_NONE;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				camera.position.y += 5.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				camera.position.y -= 5.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				mesh.rotation.y += 5.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				mesh.rotation.y -= 5.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_a) {
				camera.yaw += 2.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_d) {
				camera.yaw -= 2.0 * delta_time;
			}
			if (event.key.keysym.sym == SDLK_w) {
				camera.forward_velocity = camera.direction.mul(5.0 * delta_time);
				camera.position = camera.position.add(camera.forward_velocity);
			}
			if (event.key.keysym.sym == SDLK_s) {
				camera.forward_velocity = camera.direction.mul(5.0 * delta_time);
				camera.position = camera.position.sub(camera.forward_velocity);
			}
			break;
		}
	}
}


// Updates each frame
void App::update()
{
	PROFILE_FUNCTION();

	delta_time = (SDL_GetTicks() - old_frame_time) / 1000.0;
	old_frame_time = SDL_GetTicks();

	mesh.rotation.x += 0.0f * delta_time;
	mesh.rotation.y += 0.0f * delta_time;
	mesh.rotation.z += 0.0f * delta_time;

	//mesh.scale.x = 0.9 * delta_time;
	//mesh.scale.y = 0.9 * delta_time;
	//mesh.scale.z = 0.9 * delta_time;

	//mesh.translation.x += 1 * delta_time;
	mesh.translation.z = 5.0f;
	//mesh.translation.y = -1.0f;

	//camera.position.x += 0.0 * delta_time;
	//camera.position.y += 0.0 * delta_time;

	//initialize target looking at positive z-axis
	Vector3 up_direction = { 0, 1, 0 };
	Vector3 target = { 0, 0, 1 };
	Matrix4 yaw_rotation = Matrix4::make_rotation_y(camera.yaw);
	camera.direction = (yaw_rotation.mul_vector(target.to_vec4())).to_vec3();
	target = camera.position.add(camera.direction);

	view_matrix = Matrix4::make_third_person(camera.position, target, up_direction);

	Matrix4 scale_matrix = Matrix4::make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	Matrix4 translation_matrix = Matrix4::make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	Matrix4 rotation_matrix_x = Matrix4::make_rotation_x(mesh.rotation.x);
	Matrix4 rotation_matrix_y = Matrix4::make_rotation_y(mesh.rotation.y);
	Matrix4 rotation_matrix_z = Matrix4::make_rotation_z(mesh.rotation.z);

	Triangle3 transformed_triangle3;
	Triangle4 transformed_triangle;
	Triangle4 projected_triangle;

	for (int i = 0; i < mesh.faces.size(); i++) {
		Face& mesh_face = mesh.faces[i];

		std::array<Vector3, 3> face_vertices;
		face_vertices[0] = mesh.vertices[mesh_face.a];
		face_vertices[1] = mesh.vertices[mesh_face.b];
		face_vertices[2] = mesh.vertices[mesh_face.c];

		for (int j = 0; j < 3; j++) {
			Vector4 transformed_vertex = face_vertices[j].to_vec4();
			world_matrix = Matrix4::make_world(scale_matrix, rotation_matrix_x,
				rotation_matrix_y, rotation_matrix_z, translation_matrix);

			//transform vertex to world space
			transformed_vertex = world_matrix.mul_vector(transformed_vertex);

			//transform vertex to camera space
			transformed_vertex = view_matrix.mul_vector(transformed_vertex);

			transformed_triangle.points[j] = transformed_vertex;
		}
		transformed_triangle3 = transformed_triangle.to_triangle3();
		transformed_triangle3.compute_face_normal();


		if (cull_type == Draw::Cull_type::CULL_BACKFACE) {
			Vector3 origin(0, 0, 0);
			if (transformed_triangle3.backface(origin)) {
				continue;
			}
		}

		Polygon polygon = create_polygon_from_triangle(transformed_triangle3, mesh_face.a_uv, mesh_face.b_uv, mesh_face.c_uv);

		polygon.clip_polygon();

		std::array<Triangle3, MAX_NUM_POLY_TRIANGLES> triangles_after_clipping;
		int num_triangles_after_clipping = 0;

		if (polygon.num_vertices == 0) continue;
		polygon.triangles_from_polygon(triangles_after_clipping, num_triangles_after_clipping);

		for (size_t t = 0; t < num_triangles_after_clipping; t++) {

			Triangle3 triangle_after_clipping = triangles_after_clipping[t];

			for (int j = 0; j < 3; j++) {
				Vector4 projected_point = projection_matrix.mul_project((triangle_after_clipping.points[j]).to_vec4());

				projected_point.y *= -1;

				projected_point.x *= WINDOW_WIDTH * 0.5f;
				projected_point.y *= WINDOW_HEIGHT * 0.5f;

				projected_point.x += WINDOW_WIDTH * 0.5f;
				projected_point.y += WINDOW_HEIGHT * 0.5f;

				projected_triangle.points[j] = projected_point;
			}

			float light_factor = -transformed_triangle3.normal_deviation(global_light.direction);
			projected_triangle.tex_coords = { triangle_after_clipping.tex_coords[0], triangle_after_clipping.tex_coords[1],
				triangle_after_clipping.tex_coords[2] };
			projected_triangle.color = global_light.intensity(mesh_face.color, light_factor);
			triangles_to_render.push_back(projected_triangle);
		}
	}

}


// Renders the current display buffer
void App::render()
{
	PROFILE_FUNCTION();

	Draw::grid();

	int trianglesize = triangles_to_render.size();
	for (int i = 0; i < trianglesize; i++) {
		Triangle4 triangle4 = triangles_to_render[i];
		//Triangle2 triangle = triangle4.to_triangle2();

		if (render_type == Draw::Render_type::RENDER_FILL_TRIANGLE || render_type == Draw::Render_type::RENDER_FILL_TRIANGLE_WIREFRAME) {
			Draw::fill_triangle(triangle4, triangle4.color);
		}

		if (render_type == Draw::Render_type::RENDER_FILL_TRIANGLE_WIREFRAME || render_type == Draw::Render_type::RENDER_TEXTURED_WIRE
			|| render_type == Draw::Render_type::RENDER_WIREFRAME_VERTEX || render_type == Draw::Render_type::RENDER_WIREFRAME) {
			Draw::triangle(triangle4, 0xFFDDDDDD);
		}

		if (render_type == Draw::Render_type::RENDER_TEXTURED || render_type == Draw::Render_type::RENDER_TEXTURED_WIRE) {
			Draw::textured_triangle(triangle4, mesh_texture);
		}

		if (render_type == Draw::Render_type::RENDER_WIREFRAME_VERTEX) {
			Draw::rectangle(triangle4.points[0].x - 3, triangle4.points[0].y - 3, 6, 6, 0xFFFFFFFF);
			Draw::rectangle(triangle4.points[1].x - 3, triangle4.points[1].y - 3, 6, 6, 0xFFFFFFFF);
			Draw::rectangle(triangle4.points[2].x - 3, triangle4.points[2].y - 3, 6, 6, 0xFFFFFFFF);
		}
	}

	render_display_buffer();

	triangles_to_render.clear();
	clear_display_buffer();
	clear_z_buffer();

	SDL_RenderPresent(renderer);
}


//	Sets up the display buffer and should be called during Initialisation
void App::setup_display()
{
	display_buffer = new Color[static_cast<long long>(WINDOW_HEIGHT) * WINDOW_WIDTH];
	memset(display_buffer, 0, static_cast<long long>(WINDOW_WIDTH) * WINDOW_HEIGHT * sizeof(Color));

	z_buffer = new float[static_cast<long long>(WINDOW_HEIGHT) * WINDOW_WIDTH];
	clear_z_buffer();

	if (!display_buffer) {
		std::cout << "Could not allocate memory for buffer";
	}

	//actual color is stored as reverse RGBA (ABGR) probably because of endianness or something
	display_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);


	if (!mesh.load_obj_mesh_data("./assets/crab.obj")) {
		quit();
		return;
	}
	load_png_texture_data("./assets/crab.png");

	constexpr float fov_y = 60.0f * static_cast<float>(M_PI) / 180.0f;
	float aspect_x = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	float fov_x = atan(tan(fov_y / 2) * aspect_x) * 2;
	float aspect_y = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
	constexpr float z_near = 0.1f;
	constexpr float z_far = 100.0f;
	projection_matrix = Matrix4::make_perspective(fov_y, aspect_y, z_near, z_far);

	init_frustum_planes(fov_x, fov_y, z_near, z_far);

	//mesh.load_cube_mesh_data();


	std::cout << "done" << '\n';
}


inline void App::clear_display_buffer()
{
	memset(display_buffer, 0, static_cast<long long>(WINDOW_WIDTH) * WINDOW_HEIGHT * sizeof(Color));
}

inline void App::clear_z_buffer()
{
	memset(z_buffer, 0, static_cast<long long>(WINDOW_HEIGHT) * WINDOW_WIDTH * sizeof(float));
	/*for (int y = 0; y < WINDOW_HEIGHT; y++) {
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			z_buffer[(WINDOW_WIDTH * y) + x] = 1.0;
		}
	}*/
}


void App::quit()
{
	is_running = false;
	return;
}


void App::destroy()
{
	delete[] display_buffer;
	delete[] z_buffer;
	upng_free(png_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void App::render_display_buffer()
{
	SDL_UpdateTexture(display_buffer_texture, NULL, display_buffer, WINDOW_WIDTH * sizeof(Color));
	SDL_RenderCopy(renderer, display_buffer_texture, NULL, NULL);
}