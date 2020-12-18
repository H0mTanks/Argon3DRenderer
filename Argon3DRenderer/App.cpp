#include <iostream>
#include <vector>
#include "App.hpp"
#include "Draw.hpp"
#include "Vector.hpp"
#include "Mesh.hpp"


Vector3 camera_position = Vector3(0, 0, 0);
//Triangle triangles_to_render[N_MESH_FACES];
std::vector<Triangle2D> triangles_to_render;

Mesh mesh;

int App::WINDOW_WIDTH = 800;
int App::WINDOW_HEIGHT = 600;

SDL_Renderer* App::renderer = nullptr;
SDL_Texture* App::display_buffer_texture = nullptr;
uint32_t* App::display_buffer = nullptr;



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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
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
				break;
			}
		}
	}
}


// Updates each frame
void App::update()
{

	mesh.rotation.x += 0.01f;
	mesh.rotation.y += 0.01f;
	mesh.rotation.z += 0.01f;

	Triangle3D transformed_triangle;
	Triangle2D projected_triangle;

	for (int i = 0; i < mesh.faces.size(); i++) {
		Face& mesh_face = mesh.faces[i];

		std::array<Vector3, 3> face_vertices;
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		for (int j = 0; j < 3; j++) {
			Vector3 transformed_vertex = face_vertices[j];
			transformed_vertex = transformed_vertex.rotate_xyz(mesh.rotation.x, mesh.rotation.y, mesh.rotation.z);

			transformed_vertex.z += 100;
			transformed_triangle.points[j] = transformed_vertex;
		}

		if (transformed_triangle.backface(camera_position)) {
			continue;
		}


		for (int j = 0; j < 3; j++) {
			Vector2 projected_point = transformed_triangle.points[j].orthographic_project();

			projected_point.x += WINDOW_WIDTH / 2;
			projected_point.y += WINDOW_HEIGHT / 2;
			projected_triangle.points[j] = projected_point;
		}
		triangles_to_render.push_back(projected_triangle);
		/*std::cout << "rendering " << count << '\n';
		count++;*/
	}
}


// Renders the current display buffer
void App::render()
{
	Draw::grid();

	int trianglesize = triangles_to_render.size();

	for (int i = 0; i < trianglesize; i++) {
		Triangle2D triangle = triangles_to_render[i];
		Draw::triangle(triangle, 0xFFFFFF00);
	}


	render_display_buffer();

	triangles_to_render.clear();
	clear_display_buffer();

	SDL_RenderPresent(renderer);
}



//	Sets up the display buffer and should be called during Initialisation
void App::setup_display()
{

	display_buffer = static_cast<uint32_t*> (new uint32_t[WINDOW_WIDTH * WINDOW_HEIGHT]);
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

	if (!display_buffer) {
		std::cout << "Could not allocate memory for buffer";
	}

	display_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!mesh.load_obj_mesh_data("./assets/cube.obj")) {
		quit();
		return;
	}

	std::cout << "done" << '\n';

}


void App::clear_display_buffer()
{
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
}


void App::quit()
{
	is_running = false;
	return;
}


void App::destroy()
{
	delete display_buffer;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void App::render_display_buffer()
{
	SDL_UpdateTexture(display_buffer_texture, NULL, display_buffer, WINDOW_WIDTH * sizeof(uint32_t));
	SDL_RenderCopy(renderer, display_buffer_texture, NULL, NULL);
}